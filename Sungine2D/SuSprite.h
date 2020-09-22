#pragma once

#include "ShaderUtil.h"
#include "SuTexture2D.h"

class SuSprite
{
private:
    //Shader variable for the shader from the resource manager.
    ShaderUtil mShaderUtil;

    unsigned int mQuadVAO, IBO, mVBO, mTVBO;
    float mAlpha = 1.f;

    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();

public:
    // Constructor (inits shaders/shapes)
    SuSprite(ShaderUtil& shader);

    // Destructor
    ~SuSprite();

    // Renders a defined quad textured with given sprite
    void DrawSprite(SuTexture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(1.0f, 1.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void SetAlpha(float alpha);
};

