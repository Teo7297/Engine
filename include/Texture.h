#pragma once

#include "engine_includes.h"

namespace engine
{

    class ENGINE_API Texture
    {
    private:
        GLuint m_id = -1;
        GLuint m_slot = 0;

    public:
        Texture(const Path &filename, GLuint slot = 0);
        ~Texture();

        const inline GLuint getSlot() const { return m_slot; };

        GLuint activate();
        void bind();
        void release();
    };

}
