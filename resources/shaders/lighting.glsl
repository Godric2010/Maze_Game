#include <camera>

/// Lighting.glsl import

// Properties
// Uniforms - light
uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform float u_AmbientStrength;

// Functions
vec3 CalculateBlinnPhong(vec3 normal, vec3 frag_pos, float specular_strength, float shininess){
    vec3 N = normalize(normal);
    vec3 L = normalize(u_LightPosition - frag_pos);
    vec3 V = normalize(u_CameraPos.xyz - frag_pos);
    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0f);
    float spec = 0.0;
    if (diff > 0.0){
        spec = pow(max(dot(N, H), 0.0f), shininess);
    }

    vec3 ambient = u_AmbientStrength * u_LightColor;
    vec3 diffuse = diff * u_LightColor;
    vec3 specular = specular_strength * spec * u_LightColor;

    return ambient + diffuse + specular;
}
/// End lighting.glsl import
