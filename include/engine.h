#pragma once

#include "engine_includes.h"

class Renderer;

namespace engine
{
    class Entity;
    class ENGINE_API Engine
    {
    public:
        Engine(std::string appName);
        ~Engine();

        void close();
        void start();
        void addEntity(std::shared_ptr<Entity> e) { m_scene.push_back(e); };

    private:
    public:
    private:
        GLFWwindow *m_window;
        std::unique_ptr<Renderer> m_renderer;
        std::vector<std::shared_ptr<Entity>> m_scene;
        std::chrono::high_resolution_clock::time_point m_lastFrameTime;
    };
}