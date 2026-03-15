#include "Core/Core.h"
#include "Core/Math.h"
#include "Renderer/Camera.h"
#include "Core/Timestep.h"
#include "Events/Event.h"
#include "Events/MouseEvents.h"

namespace Velt 
{
    class EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fovY, float aspect, float near, float far);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        inline float GetDistance() const { return m_Distance; };
        inline void SetDistance(float distance) { m_Distance = distance; }; 

        inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); };
        const Matrix& GetViewMatrix() const { return m_ViewMatrix; }
        Matrix GetViewProjection() const { return m_Projection * m_ViewMatrix; };

        Vector GetUpDirection() const; 
        Vector GetRightDirection() const; 
        Vector GetForwardDirection() const; 
        const Vector& GetPosition() const { return m_Position; };
        Quaternion GetOrientation() const;

        float GetPitch() const { return m_Pitch; };
        float GetYaw() const { return m_Yaw; }

    private:
        void UpdateProjection(); 
        void UpdateView();

        bool OnMouseScroll(MouseScrolledEvent& e);

        void MousePan(const Point& delta);
        void MouseRotate(const Point& delta);
        void MouseZoom(float delta);

		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

        Vector CalculatePosition() const; 

        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed() const;

        Vector m_Position{ 0.0f };
        Vector m_FocalPoint{ 0.0f }; 

		Point m_InitialMousePosition = { 0.0f, 0.0f };


        Matrix m_ViewMatrix{1.0f};
        
        float m_ViewportWidth{}, m_ViewportHeight{}; 
        float m_Distance{ 10.0f };
        float m_Pitch{}, m_Yaw{}; 
    };
}