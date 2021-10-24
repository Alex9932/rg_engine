#version 330 core

in vec2 _coords;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 bloom;

uniform sampler2D diffuse;
uniform sampler2D lightmap;

void main () {
	color = texture(diffuse, _coords);
	color.rgb *= texture(lightmap, _coords).rgb;
	
	float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 0.4) {
		bloom = vec4(color.rgb, 1.0);
	} else {
		bloom = vec4(0.0, 0.0, 0.0, 1.0);
	}
}