#version 450 core
layout(location = 0) in vec3 aPos;

out vec3 oColor;
out vec2 oUv;

layout (std140, binding=0) uniform CamMat{
    mat4 view;
    mat4 projection;
};
uniform mat4 model;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1);
}