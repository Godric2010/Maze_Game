#version 410 core

layout(location = 0) in vec3 pos;
layout(std140) uniform Camera {
    mat4 u_View;
    mat4 u_Proj;
    vec4 u_CameraPos;
};

void main()
{
    gl_Position = u_Proj * u_View * vec4(pos, 1.0);
}
