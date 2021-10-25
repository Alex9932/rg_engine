#version 330 core

in vec3 _vertex;
in vec3 _normal;
in vec3 _tangent;
in vec2 _t_coord;
in mat3 _tg_space;

flat in int _surface_type;
in float _time;

layout (location = 0) out vec4 o_color;
layout (location = 1) out vec4 o_normal;
layout (location = 2) out vec4 o_vertex;

uniform sampler2D diffuse;
uniform sampler2D normal;

uniform vec3 cam_pos;
uniform float tiling;

vec3 sun = vec3(0.1, 1, 0.5) * 100;
const float turbidity = 0.2;

vec3 perezZenith(float t, float thetaSun) {
	const float	pi = 3.1415926;
	const vec4	cx1 = vec4(0,        0.00209, -0.00375,  0.00165);
	const vec4	cx2 = vec4(0.00394, -0.03202,  0.06377, -0.02903);
	const vec4	cx3 = vec4(0.25886,  0.06052, -0.21196,  0.11693);
	const vec4	cy1 = vec4(0.0,      0.00317, -0.00610,  0.00275);
	const vec4	cy2 = vec4(0.00516, -0.04153,  0.08970, -0.04214);
	const vec4	cy3 = vec4(0.26688,  0.06670, -0.26756,  0.15346);

	float t2 = t*t;
	float chi = (4.0 / 9.0 - t / 120.0) * (pi - 2.0 * thetaSun);
	vec4 theta = vec4 (1, thetaSun, thetaSun*thetaSun, thetaSun*thetaSun*thetaSun);

	float Y = (4.0453 * t - 4.9710) * tan ( chi ) - 0.2155 * t + 2.4192;
	float x = t2 * dot ( cx1, theta ) + t * dot ( cx2, theta ) + dot ( cx3, theta );
	float y = t2 * dot ( cy1, theta ) + t * dot ( cy2, theta ) + dot ( cy3, theta );

	return vec3 ( Y, x, y );
}

vec3 perezFunc(float t, float cosTheta, float cosGamma) {
    float  gamma      = acos ( cosGamma );
    float  cosGammaSq = cosGamma * cosGamma;
    float  aY =  0.17872 * t - 1.46303;
    float  bY = -0.35540 * t + 0.42749;
    float  cY = -0.02266 * t + 5.32505;
    float  dY =  0.12064 * t - 2.57705;
    float  eY = -0.06696 * t + 0.37027;
    float  ax = -0.01925 * t - 0.25922;
    float  bx = -0.06651 * t + 0.00081;
    float  cx = -0.00041 * t + 0.21247;
    float  dx = -0.06409 * t - 0.89887;
    float  ex = -0.00325 * t + 0.04517;
    float  ay = -0.01669 * t - 0.26078;
    float  by = -0.09495 * t + 0.00921;
    float  cy = -0.00792 * t + 0.21023;
    float  dy = -0.04405 * t - 1.65369;
    float  ey = -0.01092 * t + 0.05291;

    return vec3 ( (1.0 + aY * exp(bY/cosTheta)) * (1.0 + cY * exp(dY * gamma) + eY*cosGammaSq),
                  (1.0 + ax * exp(bx/cosTheta)) * (1.0 + cx * exp(dx * gamma) + ex*cosGammaSq),
                  (1.0 + ay * exp(by/cosTheta)) * (1.0 + cy * exp(dy * gamma) + ey*cosGammaSq));
}

vec3 perezSky(float t, float cosTheta, float cosGamma, float cosThetaSun) {
    float thetaSun = acos        ( cosThetaSun );
    vec3  zenith   = perezZenith ( t, thetaSun );
    vec3  clrYxy   = zenith * perezFunc ( t, cosTheta, cosGamma ) / perezFunc ( t, 1.0, cosThetaSun );

	clrYxy[0] *= smoothstep ( 0.0, 0.1, cosThetaSun );			// make sure when thetaSun > PI/2 we have black color
	
    return clrYxy;
}

vec4 calcSkyColor() {

	//float sunAngle = cos(time * 0.3) * 3.14159;
	//float sunAngle = time * 0.3;
	//float sunAngle = 1.5;
	float sunAngle = 1.7;

	sun = vec3(160, 150 * cos(sunAngle), 150 * sin(sunAngle));
	//sun = vec3(150, 0, 150);

	vec3 v = normalize(_vertex - cam_pos);
	vec3 l = normalize(sun);
	float _lv = dot(l, v);
	//float _lv = dot(v, l);
	
	vec3 clrYxy = perezSky(turbidity, max(-v.y, 0.0) + 0.05, _lv, l.y);
	//vec3 clrYxy = perezSky(turbidity, v.y+0.05, _lv, l.z);
                                            // now rescale Y component
    clrYxy [0] = 1.0 - exp ( -clrYxy [0] / 25.0 );

    float ratio = clrYxy [0] / clrYxy [2];  // Y / y = X + Y + Z
    vec3  XYZ;

    XYZ.x = clrYxy [1] * ratio;             // X = x * ratio
    XYZ.y = clrYxy [0];                     // Y = Y
    XYZ.z = ratio - XYZ.x - XYZ.y;          // Z = ratio - X - Y

    const vec3 rCoeffs = vec3 ( 3.240479, -1.53715, -0.49853  );
    const vec3 gCoeffs = vec3 ( -0.969256, 1.875991, 0.041556 );
    const vec3 bCoeffs = vec3 ( 0.055684, -0.204043, 1.057311 );

    vec3 color = vec3 ( dot ( rCoeffs, XYZ ), dot ( gCoeffs, XYZ ), dot ( bCoeffs, XYZ ) );

    //vec3 color = vec3(dot(rCoeffs, XYZ), dot(gCoeffs, XYZ), dot(bCoeffs, XYZ));
    //color = clrYxy;
    return vec4(clamp(color, 0.0, 1.0), 1.0);

	//return vec4(0.3, 0.5, 1.0, 1);
}

void main() {
	vec4 t_color = texture(diffuse, _t_coord * tiling);
	
	if(t_color.a < 0.9) {
		discard;
	}
	
	vec3 t_normal = texture(normal, _t_coord * tiling).xyz * 2 - 1;
	vec4 t_o_normal = vec4(_tg_space * t_normal, 1);
	//t_o_normal = vec4(_normal, 1); // DISABLE NORMAL MAPPING
	
	o_color  = t_color;
	o_normal = t_o_normal;
	o_vertex = vec4(_vertex, _surface_type);
	
	if(_surface_type == 1) {
		o_color = calcSkyColor();
	}
}