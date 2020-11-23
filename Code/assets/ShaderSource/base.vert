#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 oPosition;
out vec3 oNormal;

layout (std140, binding=0) uniform CamMat{
    mat4 view;
    mat4 projection;
    vec3 viewPos;
    vec3 lightPos;
};
uniform mat4 model;

void main(){
    oPosition = (model * vec4(aPos, 1)).rgb;
    oNormal = (transpose(inverse(model)) * vec4(aNormal, 1)).rgb;
    gl_Position = projection * view * model * vec4(aPos, 1);
}