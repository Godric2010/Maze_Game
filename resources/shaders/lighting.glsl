#include <camera>

/// Lighting.glsl import

// Properties
// Uniforms - light
const int MAX_POINT_LIGHTS = 64;

struct PointLight{
    vec4 position;
    vec4 colorIntensity;
    vec4 attenuation;
};

layout(std140) uniform LightBlock{
    vec4 ambientColorIntensity;
    ivec4 lightMeta;
    PointLight pointLights[MAX_POINT_LIGHTS];
};

// Functions
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection, float specularStrength, float shininess){
    vec3 lightVector = light.position.xyz - fragPos;
    vec3 lightDirection = normalize(lightVector);
    vec3 halfDirection = normalize(lightDirection + viewDirection);
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    float specularFactor = 0.0f;

    if (diffuseFactor > 0.0){
        specularFactor = pow(max(dot(normal, halfDirection), 0.0), shininess);
    }
    vec3 lightColor = light.colorIntensity.rgb * light.colorIntensity.a;
    vec3 diffuse = diffuseFactor * lightColor;
    vec3 specular = specularStrength * specularFactor * lightColor;
    vec3 lightContribution = diffuse + specular;

    float lightDistance = length(lightVector);
    float attentuation = 1 / (light.attenuation.x + light.attenuation.y * lightDistance + light.attenuation.z * (lightDistance * lightDistance));

    return lightContribution * attentuation;
}

vec3 CalculateLighting(vec3 normal, vec3 fragPos, float specularStrength, float shininess){
    vec3 N = normalize(normal);
    vec3 V = normalize(u_CameraPos.xyz - fragPos);
    vec3 lighting = ambientColorIntensity.rgb * ambientColorIntensity.a;
    int lightCount = min(lightMeta.x, MAX_POINT_LIGHTS);

    for (int i = 0; i < lightCount; ++i){
        lighting += CalculatePointLight(pointLights[i], N, fragPos, V, specularStrength, shininess);
    }
    return lighting;
}
/// End lighting.glsl import
