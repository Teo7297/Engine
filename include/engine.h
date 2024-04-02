#pragma once

#include "IInput.h"
#include "engine_includes.h"

class Renderer;

namespace engine
{
    class Entity;
    class ENGINE_API Engine : public IInput
    {
    public:
        Engine(std::string appName);
        ~Engine();

        void start();
        void closeApp() override;
        int getKey(int key) override;
        std::shared_ptr<Entity> findEntityByName(std::string &name);

        template <typename EntityType>
        std::shared_ptr<EntityType> makeEntity(std::string name)
        {
            static_assert(std::is_base_of<engine::Entity, EntityType>::value, "EntityType must extend engine::Entity");

            std::shared_ptr<EntityType> entity = std::make_shared<EntityType>(this, name);
            m_scene.push_back(entity);
            return entity;
        }

    private:
    public:
    private:
        unsigned int m_width, m_height;
        GLFWwindow *m_window; // do not delete manually, this is managed by glfw
        std::unique_ptr<Renderer> m_renderer;
        std::vector<std::shared_ptr<Entity>> m_scene;
        std::vector<std::shared_ptr<Entity>> m_physicsScene;
        std::chrono::high_resolution_clock::time_point m_lastFrameTime;
    };
}