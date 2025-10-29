#version 410 core

layout(location = 0) in vec3 pos;

uniform mat4 u_Proj;
uniform vec4 u_Color;

layout(location = 0) out vec4 color;

void main()
{
    gl_Position = u_Proj * vec4(pos, 1.0);
}
