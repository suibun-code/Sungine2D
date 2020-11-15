#pragma once

#include <map>
#include <vector>
#include <string>

#include "SuFont.h"
#include "ShaderUtil.h"
#include "SuTexture2D.h"

#include "ECSEntityManager.h"

//SDL
#include "SDL_image.h"
#include "SDL_ttf.h"

class ResourceManager
{
private:
	ResourceManager() {}

	static ShaderUtil LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	static SuTexture2D LoadTextureFromFile(const char* file, bool alpha);
	static SuFont LoadFontFromFile(const char* path, int size);

public:
	//Resource storage.
	static std::map<std::string, ShaderUtil> Shaders;
	static std::map<std::string, SuTexture2D> Textures;
	static std::map<std::string, SuFont> Fonts;
	static std::map<std::string, ECSEntity> Texts;

	static void LoadTextureFromFont(SuTexture2D* texture, std::string text, bool alpha, SuFont font, SDL_Color color);

	static void LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	static ShaderUtil GetShader(std::string name);

	static void LoadTexture(const char* file, bool alpha, std::string name);
	static SuTexture2D GetTexture(std::string name);

	static void LoadFont(const char* path, int size, std::string name);
	static SuFont GetFont(std::string name);

	static void AddText(std::string name, std::string input, SuFont font, glm::vec2 pos = glm::vec2(1.f), SDL_Color color = { 0, 0, 0, 255 });

	static void ClearTexts();
	static void ClearText(std::string name);

	//De-allocates all loaded resources.
	static void Clear();
	static void DestroyTextures();

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

