#include "SuSpriteRenderer.h"

void SuSpriteRenderer::initRenderData()
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
}

SuSpriteRenderer::SuSpriteRenderer(ShaderUtil& mShaderUtil)
{
	this->mShaderUtil = mShaderUtil;
	this->initRenderData();
}

SuSpriteRenderer::~SuSpriteRenderer()
{
	glDeleteVertexArrays(1, &this->mQuadVAO);
}

void SuSpriteRenderer::DrawSprite(SuTexture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    //Prepare transformations.
    this->mShaderUtil.Use();
    glm::mat4 model = glm::mat4(1.0f);

    //Translation.
    model = glm::translate(model, glm::vec3(position, 0.0f));

    //Rotation.
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    //Scale.
    //size = glm::vec2(texture.Width, texture.Height) * size;
    model = glm::scale(model, glm::vec3(size, 1.0f));

    mShaderUtil.SetMatrix4("model", model);

    //Render.
    mShaderUtil.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(mQuadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}