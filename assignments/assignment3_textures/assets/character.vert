#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
uniform float _cVTime;
out vec2 UV;
void main(){
	UV = vUV;
	vec3 ePos = vec3(sin(vPos.x + _cVTime), vPos.y + cos(_cVTime), vPos.z);
	gl_Position = vec4(ePos,1.0);
}