#include "entity.h"

namespace engine
{
    Entity::Entity(std::string name)
    {
        std::cout << "new entity! " << name << "\n";

        // Create and compile the vertex shader
        auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // Create and compile the fragment shader
        auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Link the vertex and fragment shader into a shader program
        m_shaderProgram = glCreateProgram();
        glAttachShader(m_shaderProgram, vertexShader);
        glAttachShader(m_shaderProgram, fragmentShader);
        glLinkProgram(m_shaderProgram);
        glUseProgram(m_shaderProgram);

        // Create VBO and VAO
        GLuint VBO;
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &VBO);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Specify vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void Entity::update(const float frameTime)
    {
        glUseProgram(m_shaderProgram);

        auto model =
            glm::translate(glm::identity<glm::mat4>(), position) *
            glm::mat4_cast(rotation) *
            glm::scale(glm::identity<glm::mat4>(), glm::vec3(100.f));
        auto view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        auto proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1000.0f, 1000.0f);
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "model"), 1, false, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, false, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, false, glm::value_ptr(proj));
        glUseProgram(0);
    }
}
