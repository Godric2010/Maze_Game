#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(std140) uniform Camera {
    mat4 u_View;
    mat4 u_Proj;
    vec4 u_CameraPos;
};

uniform mat4 u_Model;
uniform vec4 u_Color;

layout(location = 0) out vec4 color;
out vec2 v_UV;

void main()
{
    v_UV = uv;
    gl_Position = u_Proj * u_View * u_Model * vec4(pos, 1.0);
}
