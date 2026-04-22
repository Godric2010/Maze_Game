#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(std140) uniform Camera {
    mat4 u_View;
    mat4 u_Proj;
    vec4 u_CameraPos;
};

uniform mat4 u_Model;
uniform mat3 u_NormalMatrix;
uniform vec4 u_Color;

layout(location = 0) out vec4 color;
out vec3 v_worldPosition;
out vec3 v_worldNormal;
out vec2 v_UV;

void main()
{
    v_UV = uv;
    vec4 worldPosition = u_Model * vec4(pos, 1.0);
    v_worldPosition = worldPosition.xyz;

    v_worldNormal = normalize(u_NormalMatrix * normal);

    gl_Position = u_Proj * u_View * worldPosition;
}
