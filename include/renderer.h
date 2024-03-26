#pragma once

#include "engine_includes.h"

class Renderer
{
public:
    Renderer(/* args */);
    ~Renderer();

    void render(GLuint shader, GLuint vao, int vertices);

private:
    GLuint shaderProgram{0};
    GLuint fragmentShader{0};
    GLuint vertexShader{0};
    GLuint VBO, VAO;
};
