#include "procGen.h"

namespace artLib {
	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData sphere;
		for (int i = 0; i < numSegments; i++) {
			sphere.vertices.push_back(ew::Vec3(i, i, i); //placeholder. replace with actual vertex position calculations later
		}

		return sphere;
	}
	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData cylinder;
		for (int i = 0; i < numSegments; i++) {
			cylinder.vertices.push_back(ew::Vec3(i, i, i); //placeholder. replace with actual vertex position calculations later
		}

		return cylinder;
	}
	ew::MeshData createPlane(float width, float height, int subdivisions) {
		ew::MeshData plane;
		for (int i = 0; i < numSegments; i++) {
			plane.vertices.push_back(ew::Vec3(i, i, i); //placeholder. replace with actual vertex position calculations later
		}

		return plane;
	}
}