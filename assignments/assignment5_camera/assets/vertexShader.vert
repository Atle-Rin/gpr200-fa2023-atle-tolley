#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

out vec3 Normal;
uniform mat4 _Model;
uniform float _FOV;
uniform float _Aspect;
uniform float _Ortho;
uniform float _Near;
uniform float _Far;
uniform int _Project;
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
	if (_Project == 1) {
		float top = _Ortho/2;
		float bottom = -1 * top;
		float right = (_Ortho * _Aspect)/2;
		float left = -1 * right;
		vec4 first = vec4((2 / (right - left)), 0, 0, (-1 * (right + left) / (right - left)));
		vec4 second = vec4(0, (2 / (top - bottom)), 0, (-1 * (top + bottom) / (top - bottom)));
		vec4 third = vec4(0, 0, (2 / (_Far - _Near)), (-1 * (_Far + _Near) / (_Far - _Near)));
		projection = mat4(first, second, third, vec4(0, 0, 0, 1));
	}
	else {
		float fovRad = 180 * _FOV / 3.1415926535;
		float d = tan(fovRad / 2);
		vec4 first = vec4((1 / (d * _Aspect)), 0, 0, 0);
		vec4 second = vec4(0, (1 / d), 0, 0);
		vec4 third = vec4(0, 0, ((_Near + _Far) / (_Near - _Far)), ((2 * _Near * _Far) / (_Near - _Far)));
		projection = mat4(first, second, third, vec4(0, 0, -1, 0));
	}
	position = _Projection * position;
	
	gl_Position = position;
}