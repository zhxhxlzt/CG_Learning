#version 330 core

out vec4 FragColor;

in vec3 oColor;
in vec2 oUv;

void main(){
    FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
}