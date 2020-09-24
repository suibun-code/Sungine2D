#pragma once

#include <map>
#include <string>

#include "SuTexture2D.h"
#include "ShaderUtil.h"
#include "SuText.h"
#include "SuFont.h"

//SDL
#include "SDL_image.h"
#include "SDL_ttf.h"

class ResourceManager
{
private:
    ResourceManager() {}

    static ShaderUtil LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    static SuTexture2D LoadTextureFromFile(const char* file, bool alpha);
    static SuFont LoadFontFromFile(const char* path, int size, SDL_Color color);

public:
    //Resource storage.
    static std::map<std::string, ShaderUtil> Shaders;
    static std::map<std::string, SuTexture2D> Textures;
    static std::map<std::string, SuFont> test;

    static SuTexture2D LoadTextureFromFont(const char* text, bool alpha, SuFont font);

    static ShaderUtil LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    static ShaderUtil GetShader(std::string name);

    static SuTexture2D LoadTexture(const char* file, bool alpha, std::string name);
    static SuTexture2D GetTexture(std::string name);

    static SuFont LoadFont(const char* path, int size, SDL_Color color, std::string name);
    static SuFont GetFont(std::string name);

    //De-allocates all loaded resources.
    static void Clear();
};

