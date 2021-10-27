#version 330 core

in vec2 _coords;

out vec4 color;

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D vertex;

uniform mat4 proj;
uniform mat4 view;
uniform vec3 cam_pos;

void main() {
	vec4 t_diffuse = texture(diffuse, _coords);
	vec4 t_normal = texture(normal, _coords);
	vec4 t_vertex = texture(vertex, _coords);

	if(t_vertex.a == 30) {
		vec3 cam_to_vertex = cam_pos - t_vertex.xyz;
		vec3 reflected = normalize(reflect(normalize(cam_to_vertex), t_normal.xyz));
		
		// Ray marching
		vec3 currentRay;
		vec3 uv;
		float L = 1;
		for(int i = 0; i < 30; i++) {
			currentRay = t_vertex.xyz + reflected * L;
			
			//get uv
			vec4 pVP = proj * view * vec4(currentRay, 1.0f);
			pVP.xy = vec2(0.5f, 0.5f) + vec2(0.5f, -0.5f) * pVP.xy / pVP.w;
			uv = vec3(pVP.xy, pVP.z / pVP.w);
			
			// get depth
			vec4 n_vertex = texture(vertex, uv.xy);
			L = length(t_vertex.xyz - n_vertex.xyz);
		}
		
		vec4 n_diffuse = texture(diffuse, uv.xy);
		color = vec4(n_diffuse.rgb, 1);
	} else {
		color = t_diffuse;
	}
}