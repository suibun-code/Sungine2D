#pragma once

#include <map>
#include <vector>
#include <string>

#include "SuFont.h"
#include "SuText.h"
#include "ShaderUtil.h"
#include "SuTexture2D.h"

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
	static SuTexture2D texture;

	//Resource storage.
	static std::map<std::string, ShaderUtil> Shaders;
	static std::map<std::string, SuTexture2D> Textures;
	static std::map<std::string, SuFont> Fonts;
	static std::map<std::string, SuText*> Texts;

	static SuTexture2D LoadTextureFromFont(const char* text, bool alpha, SuFont font);

	static ShaderUtil LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	static ShaderUtil GetShader(std::string name);

	static SuTexture2D LoadTexture(const char* file, bool alpha, std::string name);
	static SuTexture2D GetTexture(std::string name);

	static SuFont LoadFont(const char* path, int size, SDL_Color color, std::string name);
	static SuFont GetFont(std::string name);

	static void AddText(std::string name, std::string input, glm::vec2 pos, SuFont font);
	static void ClearTexts();
	static void ClearText(std::string name);

	static void ClearEntities();

	//De-allocates all loaded resources.
	static void Clear();

	template<class T>
	static inline void CleanVector(std::vector<T> vec)
	{
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			delete vec[i];
			vec[i] = nullptr;
		}
		vec.clear();
		vec.shrink_to_fit();
	}
};

