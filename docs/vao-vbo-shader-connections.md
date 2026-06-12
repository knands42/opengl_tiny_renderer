# How VAO, VBO, and Shaders Connect

OpenGL is a **state machine**. Most functions don't take the object you want to operate on as an argument — they operate on whatever is **currently bound**.

Think of OpenGL as a workbench with labeled slots ("binding points"). You place a tool in a slot, and functions act on whatever is in that slot.

Key slots:
- `GL_ARRAY_BUFFER` — where a VBO sits when "active"
- The currently bound VAO
- The currently active shader program
- The currently bound texture unit

---

## How VAO "knows" about VBO

The connection is implicit, made at the moment of `glVertexAttribPointer`:

```cpp
glBindVertexArray(VAO);                    // 1. VAO is now active
glBindBuffer(GL_ARRAY_BUFFER, VBO);        // 2. VBO is now in the ARRAY_BUFFER slot
glVertexAttribPointer(0, 3, GL_FLOAT, ...); // 3. THIS records the link
glEnableVertexAttribArray(0);
```

Step 3 tells the **currently bound VAO**: "For attribute slot 0, pull data from the VBO that is *currently bound* to `GL_ARRAY_BUFFER`, using this layout."

After this, the VBO can be unbound — the VAO already remembered the VBO's ID.

---

## Anatomy of `glVertexAttribPointer`

```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//                    ^  ^  ^         ^         ^                   ^
//                    |  |  |         |         |                   offset
//                    |  |  |         |         stride
//                    |  |  |         normalize?
//                    |  |  type
//                    |  components per vertex
//                    attribute slot (location)
```

| Parameter | Meaning |
|-----------|---------|
| **location** | Attribute slot number in the VAO (0..~16). NOT a position in the VBO. |
| **size** | How many components per vertex (e.g., 3 for `vec3`) |
| **type** | Data type of each component |
| **normalize** | Whether to map int values to [0,1] / [-1,1] |
| **stride** | Bytes from one vertex's data to the next vertex's data |
| **offset** | Where in the VBO this attribute first appears |

---

## Stride vs. Offset — the key insight

**Stride** = "how far to jump to find the same attribute on the next vertex."
**Offset** = "where does this attribute first appear in the buffer."

### Tightly packed (positions only)

```
[ px py pz ] [ px py pz ] [ px py pz ] ...
  vertex 0     vertex 1     vertex 2
```

```cpp
// slot 0: position
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//                                              ^ stride = 12 bytes
//                                                                ^ offset = 0
```

### Interleaved data (position + color in same VBO)

```
[ px py pz  cr cg cb ] [ px py pz  cr cg cb ] ...
  vertex 0               vertex 1
```

Configure **two attribute slots** on the same VAO, both pointing at the same VBO:

```cpp
// slot 0: position — starts at byte 0
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// slot 1: color — starts at byte 12 (after the 3 position floats)
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

Notice:
- **Stride is the same** (6 floats) — it's the size of one whole vertex
- **Offset differs** — position at byte 0, color at byte 12

---

## How the shader connects

In the vertex shader:

```glsl
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
```

OpenGL matches **by location number**, not by name. The VAO says "slot 0 → these bytes from VBO #42"; the shader says "I want my `aPos` from slot 0." They meet via the number.

The shader **does not know the VBO exists**. It just asks the VAO for attribute N and gets data back.

---

## Vertex ↔ Fragment shader

Unlike VAO/VBO, shaders are linked **explicitly**:

```cpp
glAttachShader(program, vertexShader);
glAttachShader(program, fragmentShader);
glLinkProgram(program);
// later:
glUseProgram(program);  // puts program in the "active program" slot
```

The vertex shader's `out` variables become the fragment shader's `in` variables, matched by name.

---

## What happens during `glDrawArrays`

1. GPU reads the **currently bound VAO** → gets layout + which VBO to use
2. GPU reads the **currently active shader program** → gets vertex/fragment code
3. GPU matches VAO attribute locations to shader `in` variables by number
4. Vertex shader runs per vertex → fragment shader runs per pixel → screen

---

## Summary table

| Connection | Mechanism |
|------------|-----------|
| VAO ↔ VBO | Implicit, via "currently bound" state at `glVertexAttribPointer` |
| Vertex ↔ Fragment shader | Explicit, via `glAttachShader` + `glLinkProgram` |
| VAO ↔ Shader | Implicit, via matching attribute **location numbers** |
| Everything ↔ Draw call | Whatever is bound at `glDrawArrays` time |

**The pattern is always: bind → configure → (later) bind → draw.**
