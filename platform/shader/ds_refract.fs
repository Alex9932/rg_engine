#version 330 core

in vec2 _coords;
out vec4 color;
uniform sampler2D diffuse;
uniform sampler2D r_diffuse;
uniform sampler2D r_normal;
uniform sampler2D r_vertex;
uniform sampler2D r_lightmap;

uniform mat4 proj;
uniform mat4 view;

uniform vec3 camera_pos;

void main () {
	vec2 tex_offset = 1.0 / textureSize(diffuse, 0);
	vec4 t_diffuse = texture(r_diffuse, _coords);
	vec4 t_normal = texture(r_normal, _coords);
	vec4 t_vertex = texture(r_vertex, _coords);
	vec4 t_lightmap = texture(r_lightmap, _coords);
	
	//vec4 m_color = t_diffuse;
	
	vec3 vector = normalize(t_vertex.xyz - camera_pos);
	
	// reflect(vector, t_normal.xyz);
	// refract(vector, t_normal.xyz, 1.5);
	
	//vec3 refracted = normalize(refract(vector, t_normal.xyz, 0.5));
	vec3 refracted = normalize(reflect(vector, t_normal.xyz));
	
	vec4 projected = proj * /* view * */ vec4(refracted, 0);
	projected.xy /= projected.w;
	
	color = texture(diffuse, _coords + refracted.xy*tex_offset) + t_lightmap;
	if(t_normal.a == 0) {
		color = vec4(0);
	}
	
}