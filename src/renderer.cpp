#include "renderer.h"

Renderer::Renderer()
{
    std::cout << "Renderer initialized\n";
    glClearColor(0.f, 0.f, 0.f, 1.f);
}

Renderer::~Renderer()
{
    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::render(GLuint shader, GLuint vao, int verticesCount)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);

    glBindVertexArray(0);
    glUseProgram(0);
}