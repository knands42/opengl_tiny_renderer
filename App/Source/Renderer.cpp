#include "Renderer.h"

#include <GL/gl.h>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

auto GLLogCall(const char *function, const char *file, const int line) -> bool
{
    while (GLenum error = glGetError())
    {
        printf("[OpenGL error] (%d) at function (%s) at file (%s) at line (%d)",
               error, function, file, line);
        return false;
    }

    return true;
}

void Renderer::Clear() const
{
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray &vertexArray, const Shader &shader,
                    const Texture &texture) const
{
    shader.Bind();
    vertexArray.Bind();
    texture.Bind();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, vertexArray.GetCount()));
}

void Renderer::Draw(const VertexArray &vertexArray, const IndexBuffer &indexBuffer, const Shader &shader,
                    const Texture &texture) const
{
    shader.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();
    texture.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
