#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

uniform mat4 u_Proj;
uniform vec4 u_Color;

layout(location = 0) out vec4 color;
out vec2 v_UV;

void main()
{
    v_UV = uv;
    gl_Position = u_Proj * vec4(pos, 1.0);
}
