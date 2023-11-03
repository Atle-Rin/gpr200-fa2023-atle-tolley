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
		float subW = width / subdivisions;
		float subH = height / subdivisions;
		for (int i = 0; i <= subdivisions; i++) {
			for (int j = 0; j <= subdivisions; j++) {
				ew::Vertex vertex;
				vertex.pos.x = subW * j;
				vertex.pos.z = -1 * subH * i;
				vertex.normal = ew::Vec3(0, 1, 0);
				float u = vertex.pos.x / width;
				float v = vertex.pos.z * -1 / height;
				vertex.uv = ew::Vec2(u, v);
				plane.vertices.push_back(vertex);
			}
		}
		int columns = subdivisions + 1;
		for (int i = 0; i < subdivisions; i++) {
			for (int j = 0; j < subdivisions; j++) {
				unsigned int start = i * columns + j;
				plane.indices.push_back(start);
				plane.indices.push_back(start + 1);
				plane.indices.push_back(start + columns + 1);
				plane.indices.push_back(start);
				plane.indices.push_back(start + columns);
				plane.indices.push_back(start + columns + 1);
			}
		}

		return plane;
	}
}