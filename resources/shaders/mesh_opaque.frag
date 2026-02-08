#version 410

in vec2 v_UV;
uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;

out vec4 fragColor;

void main() {

    vec4 tex = vec4(1.0);
    if (u_UseTexture){
        tex = texture(u_Texture, v_UV).rgba;
    }

    fragColor = u_Color * tex;
}
