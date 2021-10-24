#version 330 core

const vec2 offsets[4] = vec2[](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec3 _str[1];
out vec3 _vel[1];

out vec2 tex_coord;

uniform mat4 proj;
uniform mat4 view;

void main(void) {
	mat4 vp = proj * view;
    for(int i = 0; i < 4; i++) {
		//gl_Position = vp * (gl_in[0].gl_Position+vec4(offsets[i]*_str[0].x, 0, 0));
		tex_coord = offsets[i];
		gl_Position = vp * (gl_in[0].gl_Position + vec4(offsets[i]*0.2, 0, 0));
		EmitVertex();
	}
	EndPrimitive();
}