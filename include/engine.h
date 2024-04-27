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
            m_scene.push_back(entity);
            return entity;
        }

    private:
        /// @brief For now, initializes the library and loads the font as a texture array. it also initializes VAO, VBO and vectors used for batch rendering. this has to be splitted better in future
        inline void initFreeType();
        inline void renderGUI() const;

    public:
    private:
        unsigned int m_width, m_height;
        GLFWwindow *m_window; // do not delete manually, this is managed by glfw
        std::unique_ptr<Renderer> m_renderer;
        std::vector<std::shared_ptr<Entity>> m_scene;
        std::vector<std::shared_ptr<Entity>> m_physicsScene;
        std::chrono::high_resolution_clock::time_point m_lastFrameTime;

        // Text rendering vars
        const int ARRAY_LIMIT = 250;
        std::map<GLchar, Character> m_characters;
        std::string m_fontName = "C:\\Users\\teoca\\Desktop\\testDLL\\fonts/Vera.ttf";
        GLuint m_fontTextures; // texture array with all loaded fonts
        std::shared_ptr<Shader> m_textShader;
        unsigned int VAO, VBO;
        std::vector<glm::mat4> m_transforms;
        std::vector<int> m_letterMap;
    };
}