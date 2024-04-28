#pragma once

#include "IInput.h"
#include "engine_includes.h"
#include "colors.h"

class Renderer;

namespace engine
{
    struct Character
    {
        int TextureID;        // ID handle of the glyph texture
        glm::ivec2 Size;      // Size of glyph
        glm::ivec2 Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance; // Horizontal offset to advance to next glyph
    };

    class Entity;
    class Shader;
    class ENGINE_API Engine : public IInput
    {
    public:
        Engine(std::string appName);
        ~Engine();

        void start();
        void closeApp() override;
        int getKey(int key) override;
        std::shared_ptr<Entity> findEntityByName(const std::string &name) const override;
        void renderText(const std::string text, const glm::vec3 position, const float scale = .5f, const Color color = Colors::GREEN) override;
        void setBackgroundColor(const Color color) override;

        template <typename EntityType>
        std::shared_ptr<EntityType> makeEntity(std::string name)
        {
            static_assert(std::is_base_of<engine::Entity, EntityType>::value, "EntityType must extend engine::Entity");

            std::shared_ptr<EntityType> entity = std::make_shared<EntityType>(this, name);
            entity->setID(m_nextEntityID++);
            m_scene.push_back(entity);
            return entity;
        }

    private:
        static inline void framebuffer_size_callback(GLFWwindow *window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }
        /// @brief For now, initializes the library and loads the font as a texture array. it also initializes VAO, VBO and vectors used for batch rendering. this has to be splitted better in future
        inline void initFreeType();
        inline void renderGUI();
        void imgui_debugWindow();
        void genBuffersForFBRendering();

    public:
    private:
        int m_width, m_height;
        GLFWwindow *m_window; // do not delete manually, this is managed by glfw
        std::unique_ptr<Renderer> m_renderer;
        std::vector<std::shared_ptr<Entity>> m_scene;
        std::vector<std::shared_ptr<Entity>> m_physicsScene;
        std::chrono::high_resolution_clock::time_point m_lastFrameTime;
        std::queue<uint32_t> m_entitiesToDestroy;
        uint32_t m_nextEntityID;

        // Framebuffer
        GLuint m_framebuffer{0};
        GLuint m_framebufferTexture{0};
        std::shared_ptr<Shader> m_framebufferShader{nullptr};
        GLuint m_fbVAO{0}, m_fbVBO{0}, m_fbEBO{0};
        const float m_fbVertices[30]{
            // Positions      // Texture Coords
            -1.f, -1.f, 0.0f, 0.0f, 0.0f, // bottom left
            -1.f, 1.f, 0.0f, 0.0f, 1.0f, // top left
            1.f, 1.f, 0.0f, 1.0f, 1.0f, // top right
                                        // Second triangle
            1.f, 1.f, 0.0f, 1.0f, 1.0f, // top right
            1.f, -1.f, 0.0f, 1.0f, 0.0f, // bottom right
            -1.f, -1.f, 0.0f, 0.0f, 0.0f  // bottom left
        };

        // const unsigned int m_fbIndices[6]{0, 1, 2, 2, 3, 0};

        // Text rendering vars
        const int ARRAY_LIMIT = 250;
        std::map<GLchar, Character> m_characters;
        std::string m_fontName = "C:\\Users\\teoca\\Desktop\\testDLL\\fonts/Vera.ttf";
        GLuint m_fontTextures; // texture array with all loaded fonts
        std::shared_ptr<Shader> m_textShader;
        unsigned int VAO, VBO;
        std::vector<glm::mat4> m_transforms;
        std::vector<int> m_letterMap;

        // Debug
        bool m_showDebugUI{false};
        double m_frameTime{-1.0};
    };
}