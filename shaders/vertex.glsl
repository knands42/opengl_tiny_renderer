#version 330 core

layout (location = 0) in vec3 l_Pos;
layout (location = 1) in vec3 l_Color;
layout (location = 2) in vec3 l_Texture;

out vec3 v_Color;
out vec2 v_TexCoord ;

void main()
{
    gl_Position = vec4(l_Pos, 1.0);
    v_Color = l_Color;
    v_TexCoord = l_Texture;
}