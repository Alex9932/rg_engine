#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 str;
layout (location = 2) in vec3 vel;

out vec3 _str;
out vec3 _vel;

void main() {
	_str = str;
	_vel = vel;
	gl_Position = vec4(position, 1);
}