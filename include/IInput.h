#pragma once

#include "engine_includes.h"
#include "colors.h"

namespace engine
{
    class Entity;
    class ENGINE_API IInput
    {
    public:
        virtual ~IInput(){};
        virtual int getKey(int key) = 0;
        virtual void closeApp() = 0;
        virtual std::shared_ptr<Entity> findEntityByName(const std::string &name) const = 0;
        virtual void renderText(const std::string text, const glm::vec3 position, const float scale = .5f, const Color color = Colors::GREEN) = 0;
    };
}
