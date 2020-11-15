#version 330 core

out vec4 FragColor;

in vec3 oColor;
in vec2 oUv;

uniform sampler2D oTex1;
uniform sampler2D oTex2;
void main(){
    //FragColor = vec4(0.3f, 0.1f, 0.2f, 1.0f);
//    FragColor = vec4(oColor, 1.0f);
//    FragColor = vec4(1);
    FragColor = mix(texture(oTex1, oUv), texture(oTex2, oUv), 0.5);
}