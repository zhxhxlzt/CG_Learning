#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUv;


layout (std140, binding=0) uniform CamMat{
    mat4 view;
    mat4 projection;
};
uniform mat4 model;

out vec2 oUv;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    oUv = aUv;
}