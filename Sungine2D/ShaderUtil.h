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
	//Check for errors and print if detected.
	void checkCompileErrors(unsigned int object, std::string type);

public:
	//Texture ID.
	unsigned int ID = 0;

	ShaderUtil() {}

	//Sets the current shader as active.
	ShaderUtil& Use();

	//Compiles the shader from the source code. Geometry source is optional.
	void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

	//Utility functions.
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

