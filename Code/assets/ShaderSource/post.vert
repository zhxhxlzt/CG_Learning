#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec2 oUv;


void main(){
	gl_Position = vec4(aPos, 1);
	oUv = aUv;
}