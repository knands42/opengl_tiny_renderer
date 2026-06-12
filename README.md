# OpenGL

Learning OpenGL through [LeanOpenGL](https://learnopengl.com) and The Cherno's Youtube Channel.

## Prerequisites

- CMAKE
- OpenGL

```sh
# Install OpenGL
sudo pacman -S opengl
```

### Build

```shell
cmake -B build && cmake --build build --target run
```

Or just create an alias:

```shell
alias run='cmake -B build && cmake --build build --target run'
```
