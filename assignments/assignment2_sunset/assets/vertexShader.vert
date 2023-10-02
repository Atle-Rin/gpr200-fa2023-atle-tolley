#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uvCoord;
out vec2 UV;
void main(){
	gl_Position = vec4(vPos,1.0);
	UV = uvCoord;
}