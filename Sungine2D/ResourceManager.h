#pragma once

#include <map>
#include <string>

#include "SuTexture2D.h"
#include "ShaderUtil.h"

//SDL
#include "SDL_image.h"

class ResourceManager
{
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() {}
    // loads and generates a shader from file
    static ShaderUtil loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    // loads a single texture from file
    static SuTexture2D loadTextureFromFile(const char* file, bool alpha);

    static SuTexture2D loadTextureFromFont(const char* text, bool alpha);

public:
    // resource storage
    static std::map<std::string, ShaderUtil> Shaders;
    static std::map<std::string, SuTexture2D> Textures;
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static ShaderUtil LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    // retrieves a stored sader
    static ShaderUtil GetShader(std::string name);
    // loads (and generates) a texture from file
    static SuTexture2D LoadTexture(const char* file, bool alpha, std::string name);

    //TEST
    static SuTexture2D LoadTextureFont(const char* text, bool alpha, std::string name);

    // retrieves a stored texture
    static SuTexture2D GetTexture(std::string name);
    // properly de-allocates all loaded resources
    static void Clear();
};

