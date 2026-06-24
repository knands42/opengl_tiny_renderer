#include "Renderer.h"
#include "print"

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
