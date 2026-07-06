#version 460 core

layout (location = 0) in vec3 l_Pos;
layout (location = 1) in vec2 l_Texture;

out vec3 v_Color;
out vec2 v_TexCoord ;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(l_Pos, 1.0);
    v_TexCoord = l_Texture;
}