#version 410

in vec2 v_UV;
uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;

out vec4 fragColor;

void main() {
    vec4 base_color = u_Color;
    if (u_UseTexture){
        float alpha = texture(u_Texture, v_UV).r;
        base_color.a *= alpha;
    }

    if (base_color.a <= 0.0){
        discard;
    }

    fragColor = base_color;
}
