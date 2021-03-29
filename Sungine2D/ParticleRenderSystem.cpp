#include "ParticleRenderSystem.h"

//GLM
#include "glm/glm/glm.hpp"

#include "Rendering.h"
#include "Transform.h"

#include "Core.h"

void ParticleRenderSystem::Init()
{
	//Indices mapping.
	GLuint indexData[] =
	{
		2, 3, 1,
		1, 0, 2
	};

	//Vertex positions.
	float vertices[] = {
		0.f, 0.f, //Top left.
		1.f, 0.f, //Top right.
		0.f, 1.f, //Bottom left.
		1.f, 1.f  //Bottom right.

		//REVERSED
		//1.f, 0.f,  //top right
		//0.f, 0.f,  //top left
		//0.f, 1.f,  //bottom left
		//1.f, 1.f   //bottom right
	};

	//Texture coordinates.
	float texture[] = {
		0.f, 0.f, //Top left.
		1.f, 0.f, //Top right.
		0.f, 1.f, //Bottom left.
		1.f, 1.f  //Bottom right.
	};

	//VAO.
	glGenVertexArrays(1, &this->mQuadVAO);
	glBindVertexArray(this->mQuadVAO);

	//IBO.
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	//Vertex VBO.
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, NULL, NULL);
	glEnableVertexAttribArray(0);

	//Texture VBO.
	glGenBuffers(1, &mTVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mTVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture), texture, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, NULL, NULL);
	glEnableVertexAttribArray(1);

	//Unbind buffer and vertex array.
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);

	for (auto const& entity : mEntities)
	{
		auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entity);
		auto& particle = ECSHandler::Instance()->GetComponent<Particle>(entity);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		render.shaderUtil.Use();

		if (particle.life <= 0.0f)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			continue;
		}

		//Render.
		render.shaderUtil.SetVector2f("offset", particle.position);
		render.shaderUtil.SetVector4f("color", particle.color);

		glActiveTexture(GL_TEXTURE0);
		render.texture.Bind();

		glBindVertexArray(mQuadVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void ParticleRenderSystem::Draw()
{
	for (auto const& entity : mEntities)
	{
		auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entity);
		auto& particle = ECSHandler::Instance()->GetComponent<Particle>(entity);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		render.shaderUtil.Use();

		if (particle.life <= 0.0f)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			continue;
		}

		//Render.
		render.shaderUtil.SetVector2f("offset", particle.position);
		render.shaderUtil.SetVector4f("color", particle.color);

		glActiveTexture(GL_TEXTURE0);
		render.texture.Bind();

		glBindVertexArray(mQuadVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}
