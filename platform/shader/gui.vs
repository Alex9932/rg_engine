#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 t_coord;
layout (location = 2) in vec4 color;

out vec2 t_coords;
out vec4 t_color;

uniform mat4 model;
uniform ivec2 size;

void main() {
	t_coords = t_coord;
	t_color = color;
	gl_Position = model * vec4(vertex, 0, 1);
	
	ivec2 hs = size / 2;
	gl_Position.xy -= hs;
	gl_Position.xy /= hs;
}