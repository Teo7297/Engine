#pragma once

#include "engine_includes.h"

namespace engine
{
    class Entity;
    class ENGINE_API IInput
    {
    public:
        virtual ~IInput(){};
        virtual int getKey(int key) = 0;
        virtual void closeApp() = 0;
        virtual std::shared_ptr<Entity> findEntityByName(std::string &name) const = 0;
    };
}
