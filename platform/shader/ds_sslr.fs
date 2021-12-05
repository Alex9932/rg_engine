#version 330 core

in vec2 _coords;

out vec4 color;

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D vertex;

uniform mat4 proj;
uniform mat4 view;
uniform vec3 cam_pos;

vec4 castRay(in vec3 dir, inout vec3 hitCoord, out float dDepth) {
	return vec4(1);
}

void main() {
	vec4 _t_diff = texture(diffuse, _coords);
	vec4 _t_norm = texture(normal, _coords);
	vec4 _t_vert = texture(vertex, _coords);
	
	vec3  diff      = _t_diff.rgb;
	vec3  norm      = _t_norm.xyz;
	vec3  vert      = _t_vert.rgb;
	float metallic  = _t_diff.a;
	float roughness = _t_norm.a;
	float material  = _t_vert.a;
	
	if(material == 3) {
		// Do SSLR
		
		vec3 cam_to_vertex = vert - cam_pos;
		vec3 viewDir = normalize(cam_to_vertex);
		vec3 reflected = normalize(reflect(viewDir, norm));
		
		//color = vec4(reflected, 1);
		
		vec3 currentRay;
		vec3 uv;
		float L = 1;
		for(int i = 0; i < 30; i++) {
			currentRay = vert + reflected * L;
			
			//get uv
			vec4 pVP = proj * view * vec4(currentRay, 1.0f);
			
			// Normalize coords
			uv = pVP.xyz / pVP.w;
			
			//pVP.xy = vec2(0.5f, 0.5f) + vec2(0.5f, -0.5f) * pVP.xy / pVP.w;
			//uv = vec3(pVP.xy, pVP.z / pVP.w);
			
			// get depth
			vec4 n_vertex = texture(vertex, uv.xy);
			L = length(vert - n_vertex.xyz);
		}
		
		//uv.y = 1 - uv.y;
		
		vec4 n_diffuse = texture(diffuse, uv.xy);
		color = n_diffuse;
		
		color = vec4(uv.x, uv.y, 0, 1);
		
		
	} else {
		color = vec4(diff, 1);
	}
	

/*
	vec4 t_diffuse = texture(diffuse, _coords);
	vec4 t_normal = texture(normal, _coords);
	vec4 t_vertex = texture(vertex, _coords);
	float metallic = t_diffuse.a;
	float roughness = t_normal.a;

	if(t_vertex.a == 3) {
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
	} */
}