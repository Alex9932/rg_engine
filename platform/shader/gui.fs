#version 330 core

in vec2 t_coords;
in vec4 t_color;

out vec4 o_color;

uniform bool is_text;

uniform sampler2D t_unit0;

void main() {
	if(is_text) {
		o_color = t_color * texture(t_unit0, t_coords).r;
	} else {
		vec4 c_u0 = texture(t_unit0, t_coords);
		o_color = t_color * c_u0;
	}
}