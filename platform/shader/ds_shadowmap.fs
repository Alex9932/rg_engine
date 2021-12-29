#version 330 core

in vec3 _vertex;
in vec2 _t_coord;

uniform sampler2D diffuse;
uniform float tiling;

void main() {
	vec4 t_color = texture(diffuse, _t_coord * tiling);
	if(t_color.a < 0.9) {
		discard;
	}
	
	gl_FragDepth = gl_FragCoord.z;
}
