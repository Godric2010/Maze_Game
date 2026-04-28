/// Camera.glsl import

// Properties
// Uniforms - camera
layout(std140) uniform Camera{
    mat4 u_View;
    mat4 u_Proj;
    vec4 u_CameraPos;
};

vec4 GetViewPosition(vec4 worldPosition){
    return u_Proj * u_View * worldPosition;
}

// End Camera.glsl import