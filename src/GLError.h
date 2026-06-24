#pragma once

#include "glad/gl.h"

#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

auto GLLogCall(const char *function, const char *file, int line) -> bool;
