#version 330 core

in vec2 t_coords;
in vec4 t_color;

out vec4 o_color;

uniform bool is_text;

uniform sampler2D t_unit0;

void main() {
	vec4 tex_color = texture(t_unit0, t_coords);

	if(is_text) {
		o_color = t_color * tex_color.r;
	} else {
		vec3 c_u0 = tex_color.rgb;
		o_color = t_color * vec4(c_u0, 1);
		//o_color = t_color * tex_color;
	}
}