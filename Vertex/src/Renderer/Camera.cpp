#include "Camera.h"

namespace Vertex
{

    OrthographicCamera2D::OrthographicCamera2D(float left, float right, float top, float bottom)
        : m_ViewMatrix(1.0f), m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
          m_ProjectionViewMatrix(m_ProjectionMatrix * m_ViewMatrix)
    {
        // UpdateViewMatrix();
    }

    OrthographicCamera2D::~OrthographicCamera2D() { }

    void OrthographicCamera2D::UpdateViewMatrix()
    {
        m_ViewMatrix = glm::inverse(glm::rotate(
            glm::translate(glm::mat4(1.0f), m_Position), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f)));

        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

} // namespace Vertex
