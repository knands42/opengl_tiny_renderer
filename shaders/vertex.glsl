#version 460 core

layout (location = 0) in vec3 l_Pos;
layout (location = 1) in vec3 l_Color;
layout (location = 2) in vec2 l_Texture;

out vec3 v_Color;
out vec2 v_TexCoord ;

uniform mat4 u_Transform;

void main()
{
    gl_Position = u_Transform * vec4(l_Pos, 1.0);
    v_Color = l_Color;
    v_TexCoord = l_Texture;
}