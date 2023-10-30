#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"

namespace artLib {
	//Identity matrix
	inline ew::Mat4 Identity() {
		return ew::Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};
	//Scale on x,y,z axes
	inline ew::Mat4 Scale(const ew::Vec3& s) {
		return ew::Mat4(
			s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1
		);
	};
	//Rotation around X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad) {
		const float cosA = cosf(rad);
		const float sinA = sinf(rad);
		return ew::Mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cosA, -sinA, 0.0f,
			0.0f, sinA, cosA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	//Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad) {
		const float cosA = cosf(rad);
		const float sinA = sinf(rad);
		return ew::Mat4(
			cosA, 0.0f, sinA, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sinA, 0.0f, cosA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	//Rotation around Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad) {
		const float cosA = cosf(rad);
		const float sinA = sinf(rad);
		return ew::Mat4(
			cosA, -sinA, 0.0f, 0.0f,
			sinA, cosA, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	//Translate x,y,z
	inline ew::Mat4 Translate(const ew::Vec3& t) {
		return ew::Mat4(
			1.0f, 0.0f, 0.0f, t.x,
			0.0f, 1.0f, 0.0f, t.y,
			0.0f, 0.0f, 1.0f, t.z,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); //Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		ew::Mat4 getModelMatrix() const {
			return artLib::Scale(scale);
		}
	};
	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up) {
		ew::Vec3 camDir = ew::Normalize(eye - target);
		ew::Vec3 camRight = ew::Cross(up, camDir);
		camRight = ew::Normalize(camRight);
		ew::Vec3 camUp = ew::Cross(camDir, camRight);
		ew::Vec3 camTransform((-1 * ew::Dot(camRight, eye)), (-1 * ew::Dot(camUp, eye)), (-1 * ew::Dot(camDir, eye)));
		ew::Mat4 camFinal(camRight.x, camUp.x, camDir.x, 0.0f, camRight.y, camUp.y, camDir.y, 0.0f, camRight.z, camUp.z, camDir.z, 0.0f, camTransform.x, camTransform.y, camTransform.z, 1.0f);
		return camFinal;
			//use ew::Cross for cross product!
	};
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far) {
		float top, bottom, left, right;
		top = height/2;
		bottom = -1 * top;
		right = (height * aspect)/2;
		left = -1 * right;
		ew::Vec4 first((2 / (right - left)), 0.0f, 0.0f, (-1 * (right + left) / (right - left)));
		ew::Vec4 second(0.0f, (2 / (top - bottom)), 0.0f, (-1 * (top + bottom) / (top - bottom)));
		ew::Vec4 third(0.0f, 0.0f, (2 / (far - near)), (-1 * (far + near) / (far - near)));
		ew::Mat4 projection((ew::Vec4)(first.x, second.x, third.x, 0.0f), (ew::Vec4)(first.y, second.y, third.y, 0.0f), (ew::Vec4)(first.z, second.z, third.z, 0.0f), (ew::Vec4)(first.w, second.w, third.w, 1.0f));
		return projection;
	};
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far) {
		float d = 1/(tan(fov / 2));
		ew::Vec4 first((d / aspect), 0.0f, 0.0f, 0.0f);
		ew::Vec4 second(0.0f, d, 0.0f, 0.0f);
		ew::Vec4 third(0.0f, 0.0f, ((near + far) / (near - far)), ((2 * near * far) / (near - far)));
		ew::Mat4 projection((ew::Vec4)(first.x, second.x, third.x, 0.0f), (ew::Vec4)(first.y, second.y, third.y, 0.0f), (ew::Vec4)(first.z, second.z, third.z, -1.0f), (ew::Vec4)(first.w, second.w, third.w, 0.0f));
		return projection;
	};
	
}
