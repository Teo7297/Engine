#pragma once

#include "engine_includes.h"

class Camera2D
{
public:
    Camera2D(/* args */);
    ~Camera2D();

    inline const glm::highp_mat4 getView() const;
    inline const glm::highp_mat4 getProj() const;
    inline void setScreenSize(const int width3, const int height);
private:
    int m_screenWidth, m_screenHeight;
};
