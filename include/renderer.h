#pragma once

#include "engine_includes.h"

namespace engine
{
    class Texture;
    class Shader;
}

class Renderer
{
public:
    Renderer(/* args */);
    ~Renderer();

    void render(std::shared_ptr<engine::Shader> shader, GLuint vao, int vertices, const std::shared_ptr<engine::Texture> texture = nullptr);

private:
    GLuint shaderProgram{0};
    GLuint fragmentShader{0};
    GLuint vertexShader{0};
    GLuint VBO, VAO;
};
