#version 330 core

layout (location = 0) in vec3 vertex;

out vec2 _coords;

void main () {
	_coords = vertex.xy * 0.5 + 0.5;
	gl_Position = vec4(vertex, 1);
}