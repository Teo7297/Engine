#include "rectangle.h"
#include "IInput.h"

namespace engine
{

    Rectangle::Rectangle(engine::IInput *input, std::string name) : Entity(input, name)
    {
        vertices = std::vector<float>{
            // First triangle
            // Position        // Texture coordinates
            0.f, 0.f, 0.0f, 0.0f, 0.0f, // bottom left
            0.f, 1.f, 0.0f, 0.0f, 1.0f, // top left
            1.f, 1.f, 0.0f, 1.0f, 1.0f, // top right
            // Second triangle
            1.f, 1.f, 0.0f, 1.0f, 1.0f, // top right
            1.f, 0.f, 0.0f, 1.0f, 0.0f, // bottom right
            0.f, 0.f, 0.0f, 0.0f, 0.0f  // bottom left
        };
        verticesCount = 6;
    }

    Rectangle::~Rectangle()
    {
    }
    bool Rectangle::checkCollision(Entity &other)
    {
        auto otherLeft = other.position.x;
        auto otherRight = other.position.x + other.scale.x;
        auto otherTop = other.position.y;
        auto otherBottom = other.position.y + other.scale.y;

        auto thisLeft = position.x;
        auto thisRight = position.x + scale.x;
        auto thisTop = position.y;
        auto thisBottom = position.y + scale.y;

        return (
            thisRight >= otherLeft &&
            thisLeft <= otherRight &&
            thisBottom >= otherTop &&
            thisTop <= otherBottom);
    }
}