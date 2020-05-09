#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Vertex
{
    
    OrthographicCamera2D::OrthographicCamera2D(float left, float right, float top, float bottom)
        : m_ViewMatrix(1.0f),
          m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f)
    {
        // UpdateViewMatrix();
    }

    void OrthographicCamera2D::UpdateViewMatrix()
    {
        m_ViewMatrix = glm::inverse(
            glm::rotate(
                glm::translate(
                    glm::mat4(1.0f), m_Position
                ),
                m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f)
            )
        );
    }

} // namespace Vertex
