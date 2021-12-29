#version 330 core

layout (location = 0) in vec3 vertex_;
layout (location = 3) in vec2 t_coord;
layout (location = 4) in ivec4 boneid;
layout (location = 5) in vec4 weights;

#define MAX_BONES 100
#define MAX_BONE_INFLUENCE 4

out vec3 _vertex;
out vec2 _t_coord;

uniform mat4 lightSpace;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform int anim;
uniform mat4 bonesMatrices[MAX_BONES];
uniform mat4 bonesOffsets[MAX_BONES];

void main() {
	vec3 vertex = vertex_;

	//mat4 mvp = proj * view * model;
	mat4 mvp = lightSpace * model;
	mat3 m3_model = mat3(transpose(inverse(model)));

	vec4 totalPosition = vec4(0.0f);
	if(anim == 1) {
		for(int i = 0; i < MAX_BONE_INFLUENCE; i++) {
			float weight = weights[i];
			int bone_id = boneid[i];
			
			if(bone_id == -1)
				continue;
				
			if(bone_id >= MAX_BONES) {
				totalPosition = vec4(vertex, 1.0f);
				break;
			}
			
			mat4 jointTransform = bonesMatrices[bone_id] * bonesOffsets[bone_id];
			
			vec4 localPosition = jointTransform * vec4(vertex, 1.0f);
			totalPosition += localPosition * weight;
		}
	} else {
		totalPosition = vec4(vertex, 1);
	}

	_vertex  = (model * totalPosition).xyz;
	_t_coord = t_coord;

	gl_Position = mvp * totalPosition;
}
