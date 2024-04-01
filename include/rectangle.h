#pragma once

#include "engine_includes.h"
#include "entity.h"

namespace engine
{
    class IInput;
    class ENGINE_API Rectangle : public Entity
    {
    public:
        Rectangle() = delete;
        Rectangle(engine::IInput *input, std::string name);
        virtual ~Rectangle();
        virtual void init() override { Entity::init(); };
        virtual void update(float frameTime) override { Entity::update(frameTime); };
        virtual bool checkCollision(Entity &other) override;

        void setWidth(float w) { scale.x = w; };
        void setHeight(float h) { scale.y = h; };
    };
}
