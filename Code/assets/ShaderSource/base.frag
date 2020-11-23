#version 450 core

out vec4 FragColor;

in vec3 oPosition;
in vec3 oNormal;

#define PI 3.14159625358799323846264
#define ALBEDO vec3(1f, 0.8f, 0.5f)
layout (std140, binding=0) uniform CamMat{
    mat4 view;
    mat4 projection;
    vec3 viewPos;
    vec3 lightPos;
};

float D_GGX(float dotNH, float roughness)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float denom = dotNH * dotNH * (alpha2 - 1.0) + 1.0;
    return (alpha2) / (PI * denom * denom);

}


float D_SchlicksmithGGX(float dotNL, float dotNV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float GL = dotNL / (dotNL * (1.0 - k) + k);
    float GV = dotNV / (dotNV * (1.0 - k) + k);
    return GL * GV;

}

vec3 F_Schlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0-cosTheta, 5.0);
}


vec3 specularContribution(vec3 L, vec3 V, vec3 N, vec3 F0, float metallic, float roughness)
{
    vec3 H = normalize(V + L);
    float dotNH = clamp(dot(N, H), 0.0, 1.0);
    float dotNV = clamp(dot(N, N), 0.0, 1.0);
    float dotNL = clamp(dot(N, L), 0.0, 1.0);

    vec3 lightColor = vec3(1.0);
    
    vec3 color = vec3(0.0);

    if (dotNL > 0.0)
    {
        float D = D_GGX(dotNH, roughness);
        float G = D_SchlicksmithGGX(dotNL, dotNV, roughness);
        vec3 F = F_Schlick(dotNV, F0);
        vec3 spec = D * F * G / (4.0 * dotNL * dotNV + 0.001);
        vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);
        color += (kD * ALBEDO / PI + spec) * dotNL;

    }
    return color;
}


#define USE_PBR 
void main(){
    vec3 lightDir = normalize(lightPos - oPosition);
    vec3 viewDir = normalize(viewPos - oPosition);
    vec3 normal = normalize(oNormal);


    #ifdef USE_PBR
    float metallic = 0.5;
    float roughness = 0.1;
    vec3 spec = specularContribution(lightDir, viewDir, normal, vec3(0.5), metallic, roughness);
    FragColor = vec4(ALBEDO * 0.2 + spec, 1.0);
    return;
    #endif
    
    float nl = dot(normal, lightDir);
    float nv = dot(normal, viewDir);
    float diffuse = max(nl, 0) * 0.3f;
    vec3 H = normalize(viewDir + lightDir);
    float specular = pow(max(dot(H, normal), 0), 128) * 0.5;
    FragColor = vec4((0.2 + diffuse + specular) * ALBEDO, 1);
}