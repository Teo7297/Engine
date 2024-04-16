#include "entity.h"
#include "IInput.h"
#include "shader.h"
#include "Texture.h"

namespace engine
{
    Entity::Entity(IInput *input, std::string name)
        : m_input{input}, m_name{name}, m_texture{nullptr}, m_shader{nullptr}
    {
        std::cout << "new entity! " << name << "\n";
    }

    void Entity::update(const float frameTime)
    {
        if (drawEnabled && m_shader)
        {
            m_shader->Bind();

            auto model =
                glm::translate(glm::identity<glm::mat4>(), position) *
                glm::mat4_cast(rotation) *
                glm::scale(glm::identity<glm::mat4>(), scale);
            auto view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
            auto proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1000.0f, 1000.0f);

            m_shader->SetUniform("model", model);
            m_shader->SetUniform("view", view);
            m_shader->SetUniform("projection", proj);
            m_shader->SetUniform("textureSlot", (int)(m_texture->getSlot())); // todo: it doesn't make sense to set this every frame

            m_shader->Unbind();
        }
    }
}
