#version 330 core

layout (location = 0) out vec4 o_color;

uniform vec3 color;

void main() {
	o_color = vec4(color, 1);
}