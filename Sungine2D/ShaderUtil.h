#pragma once

#include <string>

//GLEW
#include "glew.h"

//GLM
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

class ShaderUtil
{
private:
	// checks if compilation or linking failed and if so, print the error logs
	void checkCompileErrors(unsigned int object, std::string type);

public:
	// state
	unsigned int ID = 0;
	// constructor
	ShaderUtil() {}
	// sets the current shader as active
	ShaderUtil& Use();
	// compiles the shader from given source code
	void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); // note: geometry source code is optional 
	// utility functions
	void SetFloat(const char* name, float value, bool useShader = false);
	void SetInteger(const char* name, int value, bool useShader = false);
	void SetVector2f(const char* name, float x, float y, bool useShader = false);
	void SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
	void SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
	void SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
	void SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
	void SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
	void SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);
};

