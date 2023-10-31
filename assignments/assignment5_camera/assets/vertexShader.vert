#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

out vec3 Normal;
uniform mat4 _Model;
uniform mat4 _View;
uniform mat4 _Projection;

void main(){
	Normal = vNormal;
	/*
		float mag = Magnitude(v);
		if (mag == 0)
			return v;
		return v / mag;
	*/
	mat4 projection;
	vec4 position = _View * _Model * vec4(vPos, 1.0);
	position = _Projection * position;
	
	gl_Position = position;
}