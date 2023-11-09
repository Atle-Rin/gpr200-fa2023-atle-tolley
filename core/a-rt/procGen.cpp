#include "procGen.h"

namespace artLib {
	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData sphere;
		for (int i = 0; i < numSegments; i++) {
			sphere.vertices.push_back(ew::Vec3(i, i, i)); //placeholder. replace with actual vertex position calculations later
		}

		return sphere;
	}
	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData cylinder;

		float top = height / 2;
		float bottom = top * -1;
		ew::Vertex topV;
		topV.pos = ew::Vec3(0, top, 0);
		topV.normal = ew::Vec3(0, 1, 0);
		cylinder.vertices.push_back(topV);
		float angleStep = (3.14159265358979323 * 2) / numSegments;
		for (int i = 0; i <= numSegments; i++) {
			float angle = i * angleStep;
			ew::Vertex vertex;
			vertex.pos.x = cos(angle) * radius;
			vertex.pos.z = sin(angle) * radius;
			vertex.pos.y = top;
			vertex.normal = ew::Vec3(0, 1, 0);
			vertex.uv = ew::Vec2((vertex.pos.x + 1) / 2, (vertex.pos.z + 1) / 2);
			cylinder.vertices.push_back(vertex);
		}
		for (int i = 0; i <= numSegments; i++) {
			float angle = i * angleStep;
			ew::Vertex vertex;
			vertex.pos.x = cos(angle) * radius;
			vertex.pos.z = sin(angle) * radius;
			vertex.pos.y = top;
			vertex.normal = vertex.pos - topV.pos;
			vertex.uv = ew::Vec2(((float)i / numSegments), vertex.pos.y);
			cylinder.vertices.push_back(vertex);
		}
		ew::Vertex botV;
		botV.pos = ew::Vec3(0, bottom, 0);
		botV.normal = ew::Vec3(0, -1, 0);
		cylinder.vertices.push_back(botV);
		for (int i = 0; i <= numSegments; i++) {
			float angle = i * angleStep;
			ew::Vertex vertex;
			vertex.pos.x = cos(angle) * radius;
			vertex.pos.z = sin(angle) * radius;
			vertex.pos.y = bottom;
			vertex.normal = ew::Vec3(0, -1, 0);
			vertex.uv = ew::Vec2((vertex.pos.x + 1) / 2, (vertex.pos.z + 1) / 2);
			cylinder.vertices.push_back(vertex);
		}
		for (int i = 0; i <= numSegments; i++) {
			float angle = i * angleStep;
			ew::Vertex vertex;
			vertex.pos.x = cos(angle) * radius;
			vertex.pos.z = sin(angle) * radius;
			vertex.pos.y = bottom;
			vertex.normal = vertex.pos - botV.pos;
			vertex.uv = ew::Vec2(((float)i / numSegments), vertex.pos.y);
			cylinder.vertices.push_back(vertex);
		}

		unsigned int topC = 0;
		unsigned int start = 1;
		unsigned int columns = numSegments + 1;
		columns *= 2;
		for (int i = 0; i < numSegments; i++) {
			cylinder.indices.push_back(topC);
			cylinder.indices.push_back(start + i);
			cylinder.indices.push_back(start + i + 1);
		}
		for (int i = 0; i < numSegments; i++) {
			cylinder.indices.push_back(columns);
			cylinder.indices.push_back(start + i + columns);
			cylinder.indices.push_back(start + i + columns + 1);
		}
		columns += numSegments + 1;
		for (int i = 0; i < numSegments; i++) {
			cylinder.indices.push_back(start + i);
			cylinder.indices.push_back(start + i + 1);
			cylinder.indices.push_back(start + i + columns);
			cylinder.indices.push_back(start + i + 1);
			cylinder.indices.push_back(start + i + columns);
			cylinder.indices.push_back(start + i + columns + 1);
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