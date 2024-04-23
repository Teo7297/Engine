#pragma once

#include "engine_includes.h"

namespace engine
{
    class IInput;
    class Texture;
    class Shader;
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
        virtual void lateUpdate(const float frameTime){};
        virtual bool checkCollision(Entity &other) { return false; };

        GLuint getVAO() { return vao; }
        const inline std::string &getName() const { return m_name; }
        void addTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }
        std::shared_ptr<Texture> getTexture() { return m_texture; }
        void addShader(std::shared_ptr<Shader> shader) { m_shader = shader; }
        std::shared_ptr<Shader> getShader() { return m_shader; }
        inline void addComponent(std::shared_ptr<Entity> comp) { m_components.emplace_back(comp); };

    private:
    public:
        std::vector<float> vertices;

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
        std::shared_ptr<Texture> m_texture;
        std::shared_ptr<Shader> m_shader;
        std::vector<std::shared_ptr<Entity>> m_components;
    };
}