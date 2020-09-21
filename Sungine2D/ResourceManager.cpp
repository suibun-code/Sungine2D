#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

//SDL
#include "SDL_ttf.h"

#include "Core.h"

// Instantiate static variables
std::map<std::string, SuTexture2D>    ResourceManager::Textures;
std::map<std::string, ShaderUtil>       ResourceManager::Shaders;

ShaderUtil ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    ShaderUtil shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

SuTexture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
    // create texture object
    SuTexture2D texture;

    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    SDL_Surface* test;

    // load image
    test = IMG_Load(file);

    // now generate texture
    texture.Generate(test->w, test->h, test->pixels);

    // and finally free image data
    SDL_FreeSurface(test);

    return texture;
}

SuTexture2D ResourceManager::loadTextureFromFont(const char* text, bool alpha)
{
    SuTexture2D texture;
    TTF_Font* mpSDLFont = TTF_OpenFont("font/CircularStd-Black.ttf", 14);
    //SDL_Texture* mpSDLFontText = nullptr;

    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    SDL_Color textcolor = { 0, 0, 0, 255 };
    SDL_Surface* fontsurface;

    fontsurface = TTF_RenderText_Blended(mpSDLFont, text, textcolor);

    //SDL_DestroyTexture(mpSDLFontText); //de-allocate previous font texture
    //mpSDLFontText = SDL_CreateTextureFromSurface(Core::Instance()->GetRenderer(), fontsurface);

    texture.Generate(fontsurface->w, fontsurface->h, fontsurface->pixels);

    SDL_FreeSurface(fontsurface);

    return texture;
}

ShaderUtil ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

ShaderUtil ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

SuTexture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

SuTexture2D ResourceManager::LoadTextureFont(const char* text, bool alpha, std::string name)
{
    Textures[name] = loadTextureFromFont(text, alpha);
    return Textures[name];
}

SuTexture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}
