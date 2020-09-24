#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "Core.h"

//Instantiate static variables for keeping shaders and textures.
std::map<std::string, ShaderUtil> ResourceManager::Shaders;
std::map<std::string, SuTexture2D> ResourceManager::Textures;
std::map<std::string, SuFont> ResourceManager::test;

ShaderUtil ResourceManager::LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    //Retrieve vertex & fragment source code from the file path.
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        //Open the files.
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        //Read the file's buffer content into streams.
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        //Close files.
        vertexShaderFile.close();
        fragmentShaderFile.close();

        //Convert the streams into strings.
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        //Load geometry shader is present.
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

    //Create shader objects from source code.
    ShaderUtil shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);

    return shader;
}
SuTexture2D ResourceManager::LoadTextureFromFile(const char* file, bool alpha)
{
    //Create texture object.
    SuTexture2D texture;

    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    SDL_Surface* surf;

    //Load the image.
    surf = IMG_Load(file);

    //Generate the texture using the SDL_Surface* properties.
    texture.Generate(surf->w, surf->h, surf->pixels);

    //Free the SDL_Surface*.
    SDL_FreeSurface(surf);

    return texture;
}
SuFont ResourceManager::LoadFontFromFile(const char* path, int size, SDL_Color color)
{
    SuFont font;

    font.mPath = path;
    font.mSize = size;
    font.mTextColor = color;

    return font;
}

ShaderUtil ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    Shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}
ShaderUtil ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

SuTexture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
    Textures[name] = LoadTextureFromFile(file, alpha);
    return Textures[name];
}
SuTexture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

SuFont ResourceManager::LoadFont(const char* path, int size, SDL_Color color, std::string name)
{
    test[name] = LoadFontFromFile(path, size, color);
    return test[name];
}
SuFont ResourceManager::GetFont(std::string name)
{
    return test[name];
}

SuTexture2D ResourceManager::LoadTextureFromFont(const char* text, bool alpha, SuFont font)
{
    SuTexture2D texture;
    TTF_Font* ttffont = TTF_OpenFont(font.mPath, font.mSize);

    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    //Create SDL_Surface*, then apply a blend function to it, and generate a texture from it. Then free the surface and return the texture.
    SDL_Surface* fontsurface;
    fontsurface = TTF_RenderText_Blended(ttffont, text, font.mTextColor);
    texture.Generate(fontsurface->w, fontsurface->h, fontsurface->pixels);
    SDL_FreeSurface(fontsurface);

    return texture;
}

void ResourceManager::Clear()
{
    //Delete all shaders.
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);

    //Delete all textures.
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}
