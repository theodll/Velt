#pragma once

#include "Core/Core.h"
#include "Core/Math.h"
#include "Renderer/Camera.h"
#include "Core/Timestep.h"
#include "Events/Event.h"
#include "Events/MouseEvents.h"
#include "Scene/Components.h"

namespace Velt 
{
    enum class CameraMode
    {
        None,
        Flycam,
        Arcball
    };

    class VELT_API EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fovY, float aspect, float near, float far);

        void Init();

        void Focus(const Vector& focusPoint);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        bool IsActive() const { return m_IsActive; }
        void SetActive(bool active) { m_IsActive = active; }

        CameraMode GetCurrentMode() const { return m_CameraMode; }

        const TransformComponent& GetTransform() const { return m_Transform; }
        TransformComponent& GetTransform() { return m_Transform; }

        float GetDistance() const { return m_Distance; }
        void SetDistance(float distance) { m_Distance = distance; }

        const Vector& GetFocalPoint() const { return m_FocalPoint; }

        inline void SetViewportSize(float width, float height) 
        { 
            if (m_ViewportWidth == width && m_ViewportHeight == height)
                return;

            m_ViewportWidth = width; 
            m_ViewportHeight = height; 
            UpdateProjection(); 
        }

        const Matrix& GetViewMatrix() const { return m_ViewMatrix; }
        Matrix GetViewProjection() const { return m_Projection * m_ViewMatrix; }

        Vector GetUpDirection() const; 
        Vector GetRightDirection() const; 
        Vector GetForwardDirection() const; 
        Quaternion GetOrientation() const;

        float GetVerticalFOV() const { return m_VerticalFOV; }
        float GetAspectRatio() const { return m_AspectRatio; }
        float GetNearClip() const { return m_NearClip; }
        float GetFarClip() const { return m_FarClip; }

        float GetPitch() const { return m_Transform.EulerDegrees.x; }
        float GetYaw() const { return m_Transform.EulerDegrees.y; }

        float GetCameraSpeed() const;

    private:
        void UpdateProjection(); 
        void UpdateView();
        bool OnMouseScroll(MouseScrolledEvent& e);

        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);

        Vector CalculatePosition() const;

        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed() const;

        Vector m_Direction{};
        Vector m_FocalPoint{};

        float m_VerticalFOV = glm::radians(45.0f);
        float m_AspectRatio = 1.778f;
        float m_NearClip = 0.1f;
        float m_FarClip = 1000.0f;

        bool m_IsActive = true;

        bool m_Panning = false;
        bool m_Rotating = false;
        glm::vec2 m_InitialMousePosition{};

        float m_Distance = 10.0f;
        float m_NormalSpeed = 0.002f;
        
        float m_ViewportWidth = 1280.0f;
        float m_ViewportHeight = 720.0f;
        
        TransformComponent m_Transform;

        float m_PitchDelta{};
        float m_YawDelta{};
        Vector m_PositionDelta{};
        Vector m_RightDirection{};

        CameraMode m_CameraMode = CameraMode::Flycam;
        float m_MinFocusDistance = 100.0f;

        static constexpr float MIN_SPEED = 0.0005f;
        static constexpr float MAX_SPEED = 2.0f;
    };
}