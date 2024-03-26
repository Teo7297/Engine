#include "entity.h"

namespace engine
{
    Entity::Entity(std::string name)
    {
        std::cout << "new entity! " << name << "\n";
    }

    void Entity::update(const float frameTime)
    {
        glUseProgram(shaderProgram);

        auto model =
            glm::translate(glm::identity<glm::mat4>(), position) *
            glm::mat4_cast(rotation) *
            glm::scale(glm::identity<glm::mat4>(), glm::vec3(100.f));
        auto view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        auto proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1000.0f, 1000.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, false, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, false, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, false, glm::value_ptr(proj));
        glUseProgram(0);
    }
}
