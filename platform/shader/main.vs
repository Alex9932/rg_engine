#version 330 core

layout (location = 0) in vec3 vertex_;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 t_coord;

layout (location = 4) in ivec4 boneid;
layout (location = 5) in vec4 weights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

out vec3 _vertex;
out vec3 _normal;
out vec3 _tangent;
out vec2 _t_coord;

out mat3 _tg_space;

flat out int _surface_type;
flat out float _time;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform float time; // in seconds
uniform int surface_type;

uniform int anim;
uniform mat4 bonesMatrices[MAX_BONES];



void main() {
	_surface_type = surface_type;
	_time = time;

	vec3 vertex = vertex_;

	//if(surface_type == 2) {
	//	vertex.x += sin(time) * 0.1;
	//	vertex.z += cos(time) * 0.1;
	//}

	mat4 mvp = proj * view * model;
	
	mat3 m3_model = mat3(transpose(inverse(model)));

	vec4 totalPosition = vec4(0.0f);
	vec4 totalNormal = vec4(0.0f);
	if(anim == 1) {
		for(int i = 0; i < MAX_BONE_INFLUENCE; i++) {
			if(boneid[i] == -1)
				continue;
				
			if(boneid[i] >= MAX_BONES) {
				totalPosition = vec4(vertex, 1.0f);
				break;
			}
			
			mat4 jointTransform = bonesMatrices[boneid[i]];
			vec4 localPosition = jointTransform * vec4(vertex, 1.0f);
			totalPosition += localPosition * weights[i];
			vec4 localNormal = jointTransform * vec4(normal, 0);
			totalNormal += localNormal * weights[i];
		}
	} else {
		totalPosition = vec4(vertex, 1);
		totalNormal = vec4(normal, 0);
	}

	_vertex  = (model * totalPosition).xyz;
	//_normal  = normalize(m3_model * normal);
	_normal  = normalize(m3_model * totalNormal.xyz);
	_tangent = normalize(m3_model * tangent);
	_t_coord = t_coord;
	
	_tangent = normalize(_tangent - dot(_tangent, _normal) * _normal);
	vec3 biTangent = cross(_tangent, _normal);
	_tg_space = mat3(_tangent, biTangent, _normal);

	gl_Position = mvp * totalPosition;
}