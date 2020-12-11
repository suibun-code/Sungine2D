#pragma once

#include "glew.h"

struct SuTexture2D
{
	//ID of texture used to reference this particular texture.
	unsigned int ID;
	unsigned int index;

	//Texture dimensions in pixels.
	unsigned int Width, Height;

	//Texture format.
	unsigned int Internal_Format;
	unsigned int Image_Format;

	//Wrapping mode.
	unsigned int Wrap_S;
	unsigned int Wrap_T;

	//Filtering mode.
	unsigned int Filter_Min;
	unsigned int Filter_Max;

	SuTexture2D();

	//Generate texture.
	void Generate(unsigned int width, unsigned int height, const void* data);

	//Binds texture as current active GL_TEXTURE_2D texture object.
	void Bind() const;
};