#version 450 core

out vec4 FragColor;
in vec2 oUv;

uniform sampler2D texture_diffuse1;

#define AABB

void main() {
    
    vec4 color = texture(texture_diffuse1, oUv);
    color.rgb = color.rgb / (color.rgb + vec3(1));
    //color.rgb = pow(color.rgb, vec3(1 / 2.2f));
    FragColor = color;
}