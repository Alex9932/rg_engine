#version 330 core

#define LIGHT_SOURCES 4

in vec2 _coords;

out vec4 color;

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D vertex;
uniform sampler2D glow;
uniform sampler2D prew;

uniform vec3 camera_pos;

// "SUN" light
//vec3 lightColor = vec3(0.93, 0.89, 0.71);
vec3 lightColor = vec3(0, 0, 0);
vec3 lightDir = normalize(vec3(1, 1, 1));

//float ambientStrength = 0.4;
float ambientStrength = 0.1;

struct PointLight {
	vec3 position;
	vec3 color;
	vec3 attenuation;
	float radius;
};

uniform PointLight lights[LIGHT_SOURCES];
uniform samplerCube lsmap[LIGHT_SOURCES];

uniform int allowShadows;

vec3 calcLight(PointLight light, vec3 vertex, vec3 normal, float metallic, float roughness) {
	vec3 lightDir = normalize(light.position - vertex);
	vec3 l_color = max(dot(normal, lightDir), 0.0) * light.color;
	
	vec3 viewDir = normalize(camera_pos - vertex);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	
	float specularStrength = 1 - roughness;
	vec3 l_specular = specularStrength * spec * light.color;
	
	float distance    = length(light.position - vertex);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));
	
	return (l_color + l_specular) * attenuation;
	
	////return l_color*attenuation + l_specular*attenuation;
}

//float calcShadow(vec3 vertex, PointLight light, samplerCube qmap, float far_plane) {
//	if(allowShadows == 0) {
//		return 1;
//	}
//
//	vec3 fragToLight = vertex - light.position;
//	float mapped_depth = texture(qmap, fragToLight).r * far_plane;
//	float bias = 0.05;
//	if(length(fragToLight) - bias > mapped_depth) {
//		return 0;
//	}
//	
//	return 1;
//}

vec3 sampleOffsetDirections[20] = vec3[] (
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);  

float calcShadow(vec3 vertex, PointLight light, samplerCube qmap) {
	if(allowShadows == 0) {
		return 1;
	}
	
	float far_plane = light.radius;
	vec3 fragToLight = vertex - light.position;
	float bias = 0.15;
	float shadow = 0.0;
	int samples  = 20;
	
	float viewDistance = length(camera_pos - vertex);
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
	//float diskRadius = length(vertex - light.position) / far_plane / 2;
	
	for(int i = 0; i < samples; ++i) {
		float closestDepth = texture(qmap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= far_plane;
		if(length(fragToLight) - bias > closestDepth) {
			shadow += 1.0;
		}
	}
	
	return 1.0 - (shadow / float(samples));
}

void main () {
	vec4 t_prew   = texture(prew, _coords);
	
	vec4 _vertex  = texture(vertex,  _coords);
	vec4 _diffuse = texture(diffuse, _coords);
	vec4 _normal  = texture(normal,  _coords);
	vec4 _glow    = texture(glow,    _coords);
	
	vec3 t_color    = _diffuse.rgb;
	vec3 t_normal   = _normal.rgb;
	vec3 t_vertex   = _vertex.rgb;
	vec3 t_glow     = _glow.rgb;
	float metallic  = _diffuse.a;
	float roughness = _normal.a;
	
	float surface_type = _vertex.a;
	
	vec3 ambient = vec3(ambientStrength);
	
	// Calculate ambient light
	vec3 diff = max(dot(t_normal, lightDir), 0.0) * lightColor;
	
	// Calculate point lights
	for(int i = 0; i < LIGHT_SOURCES; i++) {
		if(lights[i].radius > length(t_vertex - lights[i].position)) {
			diff += (calcLight(lights[i], t_vertex, t_normal, metallic, roughness) * calcShadow(t_vertex, lights[i], lsmap[i])) + t_glow;
		}
	}
	
	vec3 result = ambient + diff;
	//color = t_prew + vec4(result, 0.0);
	color = t_prew + vec4(result, 0.0);
	
	if(surface_type > 0.5) {
		color = vec4(1);
	}
}
