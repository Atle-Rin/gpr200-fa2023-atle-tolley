#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "transformations.h"
namespace artLib {
	struct Camera {
		Camera() {};
		~Camera() {};
		ew::Vec3 position; //Camera body position
		ew::Vec3 target; //Position to look at
		float fov; //Vertical field of view in degrees
		float aspectRatio; //Screen width / Screen height
		float nearPlane; //Near plane distance (+Z)
		float farPlane; //Far plane distance (+Z)
		bool orthographic; //Perspective or orthographic?
		float orthoSize; //Height of orthographic frustum
		ew::Mat4 ViewMatrix() { //World->View
			ew::Mat4 out = artLib::LookAt(position, target, ew::Vec3(0.0f, 1.0f, 0.0f));
			return out;
		}
		ew::Mat4 ProjectionMatrix() { //View->Clip
			ew::Mat4 out;
			if (orthographic) {
				out = artLib::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
			}
			else {
				out = artLib::Perspective(fov, aspectRatio, nearPlane, farPlane);
			}
			return out;
		}
	};
}
