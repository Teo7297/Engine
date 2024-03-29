#pragma once

#include "engine_includes.h"

namespace engine
{
    class ENGINE_API IInput
    {
    public:
        virtual ~IInput(){};
        virtual int getKey(int key) = 0;
        virtual void closeApp() = 0;
    };
}
