#include "renderer.h"
#include "texture.h"
#include "shader.h"

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

void Renderer::render(std::shared_ptr<engine::Shader> shader, GLuint vao, int verticesCount, const std::shared_ptr<engine::Texture> texture)
{
    if (texture)
    {
        texture->activate();
        texture->bind();
    }

    shader->Bind();
    GL_CHECK();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);
    GL_CHECK();
    glBindVertexArray(0);
    shader->Unbind();

    if (texture)
        texture->release();

    GL_CHECK();
}