#version 410

in vec3 v_worldPosition;
in vec3 v_worldNormal;
in vec2 v_UV;

// Uniforms - camera
layout(std140) uniform Camera{
    mat4 u_View;
    mat4 u_Proj;
    vec4 u_CameraPos;
};

// Uniforms - light
uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform float u_AmbientStrength;

// Uniforms - material
uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;
uniform float u_SpecularStrength;
uniform float u_Shininess;

out vec4 fragColor;

vec3 CalculateBlinnPhong(vec3 normal, vec3 frag_pos, vec3 light_pos, vec3 cam_pos, vec3 light_col, float ambient_strength, float specular_strength, float shininess){
    vec3 N = normalize(normal);
    vec3 L = normalize(light_pos - frag_pos);
    vec3 V = normalize(cam_pos - frag_pos);
    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0f);
    float spec = 0.0;
    if (diff > 0.0){
        spec = pow(max(dot(N, H), 0.0f), shininess);
    }

    vec3 ambient = ambient_strength * light_col;
    vec3 diffuse = diff * light_col;
    vec3 specular = specular_strength * spec * light_col;

    return ambient + diffuse + specular;
}


void main() {

    vec4 tex = vec4(1.0);
    if (u_UseTexture){
        tex = texture(u_Texture, v_UV).rgba;
    }

    vec4 obj_color = u_Color * tex;
    vec3 lighting = CalculateBlinnPhong(v_worldNormal, v_worldPosition, u_LightPosition, u_CameraPos.xyz, u_LightColor, u_AmbientStrength, u_SpecularStrength, u_Shininess);

    vec3 final_color = (obj_color.rgb * lighting);

    fragColor = vec4(final_color, obj_color.a);
}
