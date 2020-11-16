#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "Core.h"

//Instantiate static variables for keeping shaders and textures.
std::map<std::string, ShaderUtil> ResourceManager::Shaders;
std::map<std::string, SuTexture2D> ResourceManager::Textures;
//std::map<std::string, std::vector<SuTexture2D>> ResourceManager::TextureSheets;
std::map<std::string, SuFont> ResourceManager::Fonts;
std::map<std::string, ECSEntity> ResourceManager::Texts;

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

//std::vector<SuTexture2D> ResourceManager::LoadTexturesFromSheetFile(const char* file, bool alpha, unsigned int spriteWidth, unsigned int spriteHeight, unsigned int rows, unsigned int columns)
//{
//	//Create texture object.
//	SuTexture2D texture;
//	std::vector<SuTexture2D> textures;
//
//	SDL_Surface* surf;
//
//	//Load the image.
//	surf = IMG_Load(file);
//
//	//for (unsigned int i = spriteHeight; i < (columns * spriteHeight); i += spriteHeight)
//	//{
//	//	for (unsigned int j = spriteWidth; j < (rows * spriteWidth); j += spriteWidth)
//	//	{
//			//Generate the texture using the SDL_Surface* properties.
//			//std::cout << "x: " << j << "\n";
//			//std::cout << "y: " << i << "\n\n";
//			texture.Generate(surf->w, surf->h, surf->pixels);
//
//			if (alpha)
//			{
//				texture.Internal_Format = GL_RGBA;
//				texture.Image_Format = GL_RGBA;
//			}
//
//			textures.push_back(texture);
//	//	}
//	//}
//
//	//Free the SDL_Surface*.
//	SDL_FreeSurface(surf);
//
//	return textures;
//}

SuFont ResourceManager::LoadFontFromFile(const char* path, int size)
{
	SuFont font;

	font.path = path;
	font.size = size;

	return font;
}

void ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	Shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
}

ShaderUtil ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

void ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	Textures[name] = LoadTextureFromFile(file, alpha);
}

SuTexture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

//void ResourceManager::LoadTextureSheet(const char* file, bool alpha, unsigned int spriteWidth, unsigned int spriteHeight, unsigned int rows, unsigned int columns, std::string name)
//{
//	TextureSheets[name] = LoadTexturesFromSheetFile(file, alpha, spriteWidth, spriteHeight, rows, columns);
//}

//std::vector<SuTexture2D> ResourceManager::GetTextureSheet(std::string name)
//{
//	return TextureSheets[name];
//}

void ResourceManager::LoadFont(const char* path, int size, std::string name)
{
	Fonts[name] = LoadFontFromFile(path, size);
}

SuFont ResourceManager::GetFont(std::string name)
{
	return Fonts[name];
}

void ResourceManager::AddText(std::string name, std::string input, SuFont font, glm::vec2 pos, SDL_Color color)
{
	ECSEntity text = ECSHandler::Instance()->CreateEntity();

	ECSHandler::Instance()->AddComponent(text, TransformComponent{ 1.f, 0.f, pos });
	ECSHandler::Instance()->AddComponent(text, RenderComponent{ ResourceManager::GetShader("sprite") });
	ECSHandler::Instance()->AddComponent(text, TextComponent{ input, font, color });

	ECSHandler::Instance()->GetComponent<EntityData>(text).name = "Text_" + name;
	ECSHandler::Instance()->GetComponent<EntityData>(text).tag = "Text";

	Texts[name] = text;
}

void ResourceManager::ClearTexts()
{
	for (std::pair<std::string, ECSEntity> element : Texts)
		ECSHandler::Instance()->DestroyEntity(element.second);

	Texts.clear();
}

void ResourceManager::ClearText(std::string name)
{
	ECSHandler::Instance()->DestroyEntity(Texts[name]);
	Texts.erase(name);
}

void ResourceManager::LoadTextureFromFont(SuTexture2D* texture, std::string text, bool alpha, SuFont font, SDL_Color color)
{
	SDL_Surface* fontsurface;
	TTF_Font* ttffont;
	ttffont = TTF_OpenFont(font.path, font.size);

	if (alpha)
	{
		texture->Internal_Format = GL_RGBA;
		texture->Image_Format = GL_RGBA;
	}

	//Create SDL_Surface*, then apply a blend function to it, and generate a texture from it. Then free the surface and return the texture.
	fontsurface = TTF_RenderText_Blended(ttffont, text.c_str(), color);

	TTF_CloseFont(ttffont);

	texture->Generate(fontsurface->w, fontsurface->h, fontsurface->pixels);

	SDL_FreeSurface(fontsurface);
}

void ResourceManager::Clear()
{
	//Delete all shaders.
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);

	//Delete all textures.
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);

	//Destroy all fonts.
	Fonts.clear();

	//Destroy all texts.
	ClearTexts();
}

void ResourceManager::DestroyTextures()
{
	//Delete all textures.
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}
