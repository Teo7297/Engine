#pragma once

#include "engine_includes.h"

namespace engine
{
    class IInput;
    class ENGINE_API Entity
    {
    public:
        Entity()
        {
            std::cerr << "[ERROR] Use Engine::makeEntity to construct entities!";
            exit(-1);
        };
        Entity(IInput *input, std::string name);
        virtual ~Entity(){};

        virtual void init(){};
        virtual void update(const float frameTime);
        virtual bool checkCollision(Entity &other) { return false; };

        GLuint getVAO() { return vao; }
        GLuint getShader() { return shaderProgram; }
        const inline std::string &getName() const { return m_name; }

    private:
    public:
        std::vector<float> vertices;

        const char *vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection; 

            void main() {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
            }
        )";

        const char *fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;
            void main() {
                FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
            }
        )";

        GLuint shaderProgram{0};
        GLuint vao{0};

        glm::vec3 position{0, 0, 0};
        glm::quat rotation{glm::identity<glm::quat>()};
        glm::vec3 scale{1.f, 1.f, 0.f};
        int verticesCount = 3;
        bool physicsEnabled = false;
        bool drawEnabled = true;

    protected:
        IInput *m_input; // do not delete manually, this is managed by the engine
        std::string m_name;
    };
}