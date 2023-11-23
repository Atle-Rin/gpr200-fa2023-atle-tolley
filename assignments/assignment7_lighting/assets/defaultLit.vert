#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out Surface{
	vec2 UV;
	vec3 worldPosition;
	vec3 worldNormal;
}vs_out;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

void main(){
	vs_out.UV = vUV;
	vec4 worldPos4 = _Model * vec4(vPos,1.0);
	vs_out.worldPosition = vec3(worldPos4.xyz);
	vec4 worldNorm4 = _Model * vec4(vNormal,0.0);
	vs_out.worldNormal = vec3(worldNorm4.xyz);
	gl_Position = _ViewProjection * _Model * vec4(vPos,1.0);
}