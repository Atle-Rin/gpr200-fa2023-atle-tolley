#version 450
struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];
uniform float _Specular;
uniform float _AmbientK;
uniform float _DiffuseK;
out vec4 FragColor;

in Surface{
	vec2 UV; //Per-fragment interpolated UV
	vec3 WorldPosition; //Per-fragment interpolated world position
	vec3 WorldNormal; //Per-fragment interpolated world normal
}fs_in;

uniform sampler2D _Texture;

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	//TODO: Lighting calculations using corrected normal
	float materialLight = _AmbientK + _DiffuseK + _Specular;
	vec3 col = texture(_Texture,fs_in.UV).rgb * materialLight;
	FragColor = vec4(col, 1.0);
}