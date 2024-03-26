#pragma once

#include "engine_includes.h"

struct Color
{
    float r;
    float g;
    float b;
};

#define DEFINE_COLOR(NAME, R, G, B) \
    static constexpr Color NAME = Color { R, G, B }

class Colors
{
public:
    DEFINE_COLOR(RED, 1.0f, 0.0f, 0.0f);
    DEFINE_COLOR(GREEN, 0.0f, 1.0f, 0.0f);
    DEFINE_COLOR(BLUE, 0.0f, 0.0f, 1.0f);
    DEFINE_COLOR(CYAN, 0.0f, 1.0f, 1.0f);
    DEFINE_COLOR(MAGENTA, 1.0f, 0.0f, 1.0f);
    DEFINE_COLOR(YELLOW, 1.0f, 1.0f, 0.0f);
    DEFINE_COLOR(BLACK, 0.0f, 0.0f, 0.0f);
    DEFINE_COLOR(WHITE, 1.0f, 1.0f, 1.0f);
    DEFINE_COLOR(GRAY, 0.5f, 0.5f, 0.5f);
    DEFINE_COLOR(LIGHT_GRAY, 0.75f, 0.75f, 0.75f);
    DEFINE_COLOR(DARK_GRAY, 0.25f, 0.25f, 0.25f);
    DEFINE_COLOR(BROWN, 0.6f, 0.4f, 0.2f);
    DEFINE_COLOR(ORANGE, 1.0f, 0.5f, 0.0f);
    DEFINE_COLOR(PINK, 1.0f, 0.75f, 0.8f);
    DEFINE_COLOR(PURPLE, 0.5f, 0.0f, 0.5f);
    DEFINE_COLOR(INDIGO, 0.3f, 0.0f, 0.5f);
    DEFINE_COLOR(TEAL, 0.0f, 0.5f, 0.5f);
    DEFINE_COLOR(LIME, 0.75f, 1.0f, 0.0f);
    DEFINE_COLOR(OLIVE, 0.5f, 0.5f, 0.0f);
    DEFINE_COLOR(MAROON, 0.5f, 0.0f, 0.0f);

    static Color RandomColor()
    {
        return Color{glm::linearRand(0.f, 1.f), glm::linearRand(0.f, 1.f), glm::linearRand(0.f, 1.f)};
    }
};
