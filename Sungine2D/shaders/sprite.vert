#version 330 core

//layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = texCoords.xy;
    gl_Position = projection * view * model * vec4(position.xy, 0.0, 1.0);
}