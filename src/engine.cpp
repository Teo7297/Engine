#include "engine.h"
#include "renderer.h"
#include "entity.h"

namespace engine
{
    Engine::Engine(std::string appName)
        : m_window{nullptr},
          m_renderer{nullptr},
          m_scene{},
          m_lastFrameTime{}
    {
        // Initialize GLFW
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }

        // Create a GLFW window
        m_window = glfwCreateWindow(800, 600, appName.c_str(), NULL, NULL);
        if (!m_window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(m_window);

        // Initialize GLEW
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return;
        }

        m_renderer = std::make_unique<Renderer>();
    }

    Engine::~Engine()
    {
    }

    // ################ PUBLIC ################
    void Engine::close()
    {
        glfwSetWindowShouldClose(m_window, true);
    }

    void Engine::start()
    {
        m_lastFrameTime = std::chrono::high_resolution_clock::now();

        for (auto e : m_scene)
        {
            e->init();
        }

        while (!glfwWindowShouldClose(m_window))
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> frameTime = endTime - m_lastFrameTime;
            m_lastFrameTime = endTime;
            // std::cout << 1000.0 / frameTime.count() << " FPS\n";
            for (auto e : m_scene)
            {
                e->update(frameTime.count());
                m_renderer->render(e->getShader(), e->getVAO(), e->verticesCount);
            }

            glfwPollEvents();
            glfwSwapBuffers(m_window);
        }

        glfwTerminate();
    }

    // ################ PRIVATE ################

}
