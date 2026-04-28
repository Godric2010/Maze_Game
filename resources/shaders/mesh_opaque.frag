#version 410
#include <lighting>

in vec3 v_worldPosition;
in vec3 v_worldNormal;
in vec2 v_UV;

// Uniforms - material
uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;
uniform float u_SpecularStrength;
uniform float u_Shininess;

out vec4 fragColor;

void main() {

    vec4 tex = vec4(1.0);
    if (u_UseTexture){
        tex = texture(u_Texture, v_UV).rgba;
    }

    vec4 obj_color = u_Color * tex;
    vec3 lighting = CalculateBlinnPhong(v_worldNormal, v_worldPosition, u_SpecularStrength, u_Shininess);

    vec3 final_color = (obj_color.rgb * lighting);

    fragColor = vec4(final_color, obj_color.a);
}
