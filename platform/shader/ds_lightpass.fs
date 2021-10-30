#version 330 core

in vec2 _coords;

out vec4 color;

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D vertex;
uniform sampler2D prew;

uniform vec3 camera_pos;

// "SUN" light
//vec3 lightColor = vec3(0.93, 0.89, 0.71);
vec3 lightColor = vec3(0, 0, 0);
vec3 lightDir = normalize(vec3(0.1, 1, 0.5));

//float ambientStrength = 0.4;
float ambientStrength = 0.1;

struct PointLight {
	vec3 position;
	vec3 color;
	vec3 attenuation;
};

uniform vec3 lposition_0;
uniform vec3 lcolor_0;
uniform vec3 lattenuation_0;
uniform float lradius_0;

uniform vec3 lposition_1;
uniform vec3 lcolor_1;
uniform vec3 lattenuation_1;
uniform float lradius_1;

uniform vec3 lposition_2;
uniform vec3 lcolor_2;
uniform vec3 lattenuation_2;
uniform float lradius_2;

uniform vec3 lposition_3;
uniform vec3 lcolor_3;
uniform vec3 lattenuation_3;
uniform float lradius_3;

uniform samplerCube lsmap[4];

uniform int allowShadows;

PointLight _light0;
PointLight _light1;
PointLight _light2;
PointLight _light3;

vec3 calcLight(PointLight light, vec3 vertex, vec3 normal) {
	vec3 lightDir = normalize(light.position - vertex);
	vec3 l_color = max(dot(normal, lightDir), 0.0) * light.color;
	
	vec3 viewDir = normalize(camera_pos - vertex);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	
	float specularStrength = 0.5;
	vec3 l_specular = specularStrength * spec * light.color;
	
	float distance    = length(light.position - vertex);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));
	
	return (l_color + l_specular) * attenuation;
	//return l_color*attenuation + l_specular*attenuation;
}

float calcShadow(vec3 vertex, PointLight light, samplerCube qmap, float far_plane) {
//	vec3 fragToLight = vertex - light.position;
//	float closestDepth = texture(qmap, fragToLight).r;
//	closestDepth *= far_plane;
//	float currentDepth = length(fragToLight);
//	float bias = 0.005;
//	float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;
//	return shadow;

	if(allowShadows == 0) {
		return 1;
	}

	vec3 fragToLight = vertex - light.position;
	float mapped_depth = texture(qmap, fragToLight).r * far_plane;
	float bias = 0.05;
	if(length(fragToLight) - bias > mapped_depth) {
		return 0;
	}
	
	return 1;
}

void main () {
	_light0.position = lposition_0;
	_light0.color = lcolor_0;
	_light0.attenuation = lattenuation_0;
	_light1.position = lposition_1;
	_light1.color = lcolor_1;
	_light1.attenuation = lattenuation_1;
	_light2.position = lposition_2;
	_light2.color = lcolor_2;
	_light2.attenuation = lattenuation_2;
	_light3.position = lposition_3;
	_light3.color = lcolor_3;
	_light3.attenuation = lattenuation_3;
	
	vec4 t_prew   = texture(prew, _coords);
	vec3 t_color  = texture(diffuse, _coords).rgb;
	vec3 t_normal = texture(normal, _coords).rgb;
	vec3 t_vertex = texture(vertex, _coords).rgb;
	
	float surface_type = texture(vertex, _coords).a;
	
	vec3 ambient = vec3(ambientStrength);
	
	// Calculate ambient light
	vec3 diff = max(dot(t_normal, lightDir), 0.0) * lightColor;
	
	// Calculate point lights
	
	if(lradius_0 > length(t_vertex - lposition_0)) {
		diff += calcLight(_light0, t_vertex, t_normal) * calcShadow(t_vertex, _light0, lsmap[0], lradius_0);
	}
	if(lradius_1 > length(t_vertex - lposition_1)) {
		diff += calcLight(_light1, t_vertex, t_normal) * calcShadow(t_vertex, _light1, lsmap[1], lradius_1);
	}
	if(lradius_2 > length(t_vertex - lposition_2)) {
		diff += calcLight(_light2, t_vertex, t_normal) * calcShadow(t_vertex, _light2, lsmap[2], lradius_2);
	}
	if(lradius_3 > length(t_vertex - lposition_3)) {
		diff += calcLight(_light3, t_vertex, t_normal) * calcShadow(t_vertex, _light3, lsmap[3], lradius_3);
	}
	
	vec3 result = ambient + diff;
	color = t_prew + vec4(result, 0.0);
	
	if(surface_type > 0.5) {
		color = vec4(1);
	}
}