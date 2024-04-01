#include "rectangle.h"
#include "IInput.h"

namespace engine
{

    Rectangle::Rectangle(engine::IInput *input, std::string name) : Entity(input, name)
    {
        vertices = std::vector<float>{
            0.f, 0.f, 0.0f,  // top left
            0.f, -1.f, 0.0f, // bottom left
            1.f, -1.f, 0.0f, // bottom right
                             // Second triangle
            1.f, -1.f, 0.0f, // bottom right
            1.f, 0.f, 0.0f,  // top right
            0.f, 0.f, 0.0f   // top left
        };
        verticesCount = 6;
    }

    Rectangle::~Rectangle()
    {
    }
    bool Rectangle::checkCollision(const Entity &other)
    {
        auto otherLeft = other.position.x;
        auto otherRight = other.position.x + other.scale.x;
        auto otherTop = other.position.y;
        auto otherBottom = other.position.y + other.scale.y;

        auto thisLeft = position.x;
        auto thisRight = position.x + scale.x;
        auto thisTop = position.y;
        auto thisBottom = position.y + scale.y;

        if (thisRight >= otherLeft && thisLeft <= otherRight && thisBottom >= otherTop && thisTop <= otherBottom)
        {
            std::cout << "collision!\n";
            return true;
        }
        return false;
    }
}