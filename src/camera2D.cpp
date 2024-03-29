#include "camera2D.h"

Camera2D::Camera2D(/* args */)
    : m_screenWidth{800}, m_screenHeight{600}
{
}

Camera2D::~Camera2D()
{
}

const glm::highp_mat4 Camera2D::getView() const
{
    return glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

const glm::highp_mat4 Camera2D::getProj() const
{
    return glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1000.0f, 1000.0f);
}

inline void Camera2D::setScreenSize(const int width, const int height)
{
    if (m_screenWidth > 0)
        m_screenWidth = width;
    if (m_screenHeight > 0)
        m_screenHeight = height;
}
