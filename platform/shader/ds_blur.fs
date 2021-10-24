#version 330 core

in vec2 _coords;
out vec4 color;
uniform sampler2D diffuse;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
uniform bool horizontal;
uniform float blur_mul = 2;

void main () {
	vec2 tex_offset = 1.0 / textureSize(diffuse, 0) * blur_mul;
	vec3 result = texture(diffuse, _coords).rgb * weight[0];
	
	if(horizontal) {
		for(int i = 1; i < 5; ++i) {
			result += texture(diffuse, _coords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			result += texture(diffuse, _coords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		}
	} else {
		for(int i = 1; i < 5; ++i) {
			result += texture(diffuse, _coords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
			result += texture(diffuse, _coords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
		}
	}
	color = vec4(result, 1.0);
}