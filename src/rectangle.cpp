#include "rectangle.h"
#include "IInput.h"

namespace engine
{

    Rectangle::Rectangle(engine::IInput *input, std::string name) : Entity(input, name)
    {
        vertices = std::vector<float>{
            0.f, 0.f, 0.0f, // bottom left
            0.f, 1.f, 0.0f, // top left
            1.f, 1.f, 0.0f, // top right
                            // Second triangle
            1.f, 1.f, 0.0f, // top right
            1.f, 0.f, 0.0f, // bottom right
            0.f, 0.f, 0.0f  // bottom left

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
            std::cout << "collision! " << m_name << "\n";
            std::cout << scale.x << ", " << scale.y << " - " << other.scale.x << ", " << other.scale.y << "\n";
            std::cout << position.x << ", " << position.y << " - " << other.position.x << ", " << other.position.y << "\n";
            std::cout << thisLeft << " - " << otherLeft << "\n";
            std::cout << thisRight << " - " << otherRight << "\n";
            std::cout << thisTop << " - " << otherTop << "\n";
            std::cout << thisBottom << " - " << otherBottom << "\n";
            return true;
        }
        return false;
    }
}