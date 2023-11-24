#version 450
struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4

in Surface{
	vec2 UV; //Per-fragment interpolated UV
	vec3 WorldPosition; //Per-fragment interpolated world position
	vec3 WorldNormal; //Per-fragment interpolated world normal
}fs_in;

out vec4 FragColor;

uniform sampler2D _Texture;
uniform Light _Lights[MAX_LIGHTS];
uniform float _Specular;
uniform float _AmbientK;
uniform float _DiffuseK;
uniform float _Shininess;
uniform vec3 _CamPos;
uniform int _NumLights;

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	vec3 col = texture(_Texture,fs_in.UV).rgb;
	//TODO: Lighting calculations using corrected normal
	float dRed = 0, dGreen = 0, dBlue = 0;
	for (int i = 0; i < _NumLights; i++) {
		vec3 lightDir = _Lights[i].position - fs_in.WorldPosition;
		vec3 lightMag = normalize(lightDir) / length(lightDir);
		vec3 dColors = _Lights[i].color;
		float diffuse = dot(lightMag, normal);
		dColors *= max(diffuse, 0);
		dRed += dColors.x;
		dGreen += dColors.y;
		dBlue += dColors.z;
	}
	dRed /= _NumLights;
	dGreen /= _NumLights;
	dBlue /= _NumLights;
	float sRed = 0, sGreen = 0, sBlue = 0;
	for (int i = 0; i < _NumLights; i++) {
		vec3 lightDir = _Lights[i].position - fs_in.WorldPosition;
		vec3 lightMag = normalize(lightDir) / length(lightDir);
		vec3 camDir = _CamPos - fs_in.WorldPosition;
		vec3 camMag = normalize(camDir) / length(camDir);
		vec3 sColors = _Lights[i].color;
		vec3 halfie = lightMag + camMag;
		halfie /= normalize(halfie);
		float specular = dot(halfie, normal);
		float specFin = pow(specular, (_Shininess / 256));
		specFin = max(specFin, 0);
		sColors *= specFin;
		sRed += sColors.x;
		sGreen += sColors.y;
		sBlue += sColors.z;
	}
	sRed /= _NumLights;
	sGreen /= _NumLights;
	sBlue /= _NumLights;

	vec3 difCol = col * vec3(dRed, dGreen, dBlue);
	vec3 specCol = col * vec3(sRed, sGreen, sBlue);
	vec3 finCol = (col * _AmbientK) + (_DiffuseK * difCol) + (_Specular * specCol);
	FragColor = vec4(finCol, 1.0);
}