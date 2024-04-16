#include "engine.h"
#include "renderer.h"
#include "entity.h"
#include "shader.h"
#include "Texture.h"

namespace engine
{
    Engine::Engine(std::string appName)
        : m_width{800},
          m_height{600},
          m_window{nullptr},
          m_renderer{nullptr},
          m_scene{},
          m_physicsScene{},
          m_lastFrameTime{}
    {
        // Initialize GLFW
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }
        // Set GLFW window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL 4.6
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile

        // Create a GLFW window
        m_window = glfwCreateWindow(m_width, m_height, appName.c_str(), NULL, NULL);
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
    void Engine::start()
    {
        m_lastFrameTime = std::chrono::high_resolution_clock::now();

        for (auto e : m_scene)
        {
            if (e->drawEnabled)
            {
                auto shader = e->getShader();
                shader->Bind();

                // Create VBO and VAO
                GLuint VBO;
                glGenVertexArrays(1, &(e->vao));
                glGenBuffers(1, &VBO);

                glBindVertexArray(e->vao);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, e->vertices.size() * sizeof(float), e->vertices.data(), GL_STATIC_DRAW);

                // Specify vertex attributes
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);

                shader->Unbind();
            }

            if (e->physicsEnabled)
                m_physicsScene.push_back(e);
            e->init();
        }

        while (!glfwWindowShouldClose(m_window))
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> frameTime = endTime - m_lastFrameTime;
            m_lastFrameTime = endTime;
            // std::cout << 1000.0 / frameTime.count() << " FPS\n";
            glClear(GL_COLOR_BUFFER_BIT);
            for (auto e : m_scene)
            {
                e->update(frameTime.count());
                if (e->drawEnabled)
                    m_renderer->render(e->getShader(), e->getVAO(), e->verticesCount, e->getTexture());
            }
            for (auto e : m_physicsScene)
            {
                for (auto e2 : m_physicsScene)
                {
                    if (e != e2)
                        e->checkCollision(*e2);
                }
            }

            glfwPollEvents();
            glfwSwapBuffers(m_window);
        }

        glfwTerminate();
    }

    // # INTERFACE METHODS
    void Engine::closeApp()
    {
        glfwSetWindowShouldClose(m_window, true);
    }

    int Engine::getKey(int key)
    {
        return glfwGetKey(m_window, key);
    }

    std::shared_ptr<Entity> Engine::findEntityByName(const std::string &name) const
    {
        for (auto e : m_scene)
        {
            if (name == e->getName())

                return e;
        }
        return nullptr;
    }
    // ################ PRIVATE ################
}
