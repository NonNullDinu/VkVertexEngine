#pragma once

#include "Core/Core.h"

namespace Vertex
{

    class Camera
    {
    public:
        virtual const glm::vec3& GetPosition() const = 0;
        virtual void SetPosition(const glm::vec3& position) = 0;

        virtual const glm::mat4& GetViewMatrix() const = 0;
        virtual const glm::mat4& GetProjectionMatrix() const = 0;

        virtual const glm::mat4& GetProjectionViewMatrix() const = 0;
    };

    class Camera2D : public Camera
    {
    public:
        virtual const float& GetRotation() const = 0;
        virtual void SetRotation(const float rotation) = 0;
    };

    class OrthographicCamera2D : public Camera2D
    {
    public:
        OrthographicCamera2D(float left, float right, float top, float bottom);
        ~OrthographicCamera2D();

        inline const glm::vec3& GetPosition() const override
        { return m_Position; }
        inline void SetPosition(const glm::vec3& position) override
        {
            m_Position = position;
            UpdateViewMatrix();
        }

        inline const float& GetRotation() const override
        { return m_Rotation; }
        inline void SetRotation(const float rotation) override
        {
            m_Rotation = rotation;
            UpdateViewMatrix();
        }

        inline const glm::mat4& GetViewMatrix() const override
        { return m_ViewMatrix; }
        inline const glm::mat4& GetProjectionMatrix() const override
        { return m_ProjectionMatrix; }

        inline const glm::mat4& GetProjectionViewMatrix() const override
        { return m_ProjectionViewMatrix; }

    private:
        glm::vec3 m_Position;
        float m_Rotation = 0.0f;

        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;

        glm::mat4 m_ProjectionViewMatrix; // cache

    private:
        void UpdateViewMatrix();
    };

} // namespace Vertex
