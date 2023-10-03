#pragma once
#include "C:\Users\atle.tolley\GPR-200 Repo\gpr200-fa2023-atle-tolley\core\ew\external\glad.h"
#include <sstream>
#include <fstream>
namespace artLib {
	std::string loadShaderSourceFromFile(const std::string& filePath);
	unsigned int createShader(GLenum shaderType, const char* sourceCode);
	unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

	class Shader {
	public:
		Shader(const std::string& vertexShader, const std::string& fragmentShader);

		void use();
		void setInt(const std::string& name, int v) const;
		void setFloat(const std::string& name, float v) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;
		unsigned int getMID() { return m_id; }
	private:
		unsigned int m_id;
	};
}