#include "renderer.h"

// Macro to check for OpenGL errors
#define GL_CHECK()                                                                                                         \
    do                                                                                                                     \
    {                                                                                                                      \
        GLenum err = glGetError();                                                                                         \
        if (err != GL_NO_ERROR)                                                                                            \
        {                                                                                                                  \
            std::cerr << "OpenGL error (" << __FILE__ << ":" << __LINE__ << "): " << glewGetErrorString(err) << std::endl; \
        }                                                                                                                  \
    } while (0)

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
    if (!vao || !shader || !verticesCount)
        return;
    glUseProgram(shader);
    GL_CHECK();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);
    GL_CHECK();
    glBindVertexArray(0);
    glUseProgram(0);
    GL_CHECK();
}