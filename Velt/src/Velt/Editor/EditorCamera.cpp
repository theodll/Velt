#include "vtpch.h"
#include "EditorCamera.h"
#include "Core/Input.h"
#include "Events/EventDispatcher.h"
#include "Events/MouseEvents.h"

namespace Velt 
{
    namespace
    {
        float ToRadiansIfNeeded(float fov)
        {
            return fov > glm::pi<float>() ? glm::radians(fov) : fov;
        }

        void DisableMouse()
        {
            Input::LockMouse();
        }

        void EnableMouse()
        {
            Input::UnlockMouse();
        }
    }

    EditorCamera::EditorCamera(float fovY, float aspect, float near, float far)
        : m_VerticalFOV(ToRadiansIfNeeded(fovY)),
          m_AspectRatio(aspect),
          m_NearClip(near), 
          m_FarClip(far),
          Camera(glm::perspective(m_VerticalFOV, aspect, near, far))
    {
        VT_PROFILE_FUNCTION();
        Init();
    }

    void EditorCamera::Init()
    {
        VT_PROFILE_FUNCTION();
        constexpr glm::vec3 position = { -5.0f, 5.0f, 5.0f };
        m_Position = position;
        m_FocalPoint = { 0.0f, 0.0f, 0.0f };
        m_Distance = glm::distance(position, m_FocalPoint);

        m_Yaw = 3.0f * glm::pi<float>() / 4.0f;
        m_Pitch = glm::pi<float>() / 4.0f;

        m_Position = CalculatePosition();
        m_Direction = GetForwardDirection();
        m_RightDirection = GetRightDirection();
        m_InitialMousePosition = { Input::GetMouseX(), Input::GetMouseY() };

        UpdateProjection();
        UpdateView();
    }

    void EditorCamera::UpdateProjection()
    {
        VT_PROFILE_FUNCTION();
        if (m_ViewportHeight <= 0.0f)
            return;

        m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
        m_Projection = glm::perspective(m_VerticalFOV, m_AspectRatio, m_NearClip, m_FarClip);
    }

    void EditorCamera::OnUpdate(Timestep ts)
    {
        VT_PROFILE_FUNCTION();
        const glm::vec2 mouse{ Input::GetMouseX(), Input::GetMouseY() };
        const glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.002f;

        if (!m_IsActive)
        {
            EnableMouse();
            m_InitialMousePosition = mouse;
            return;
        }

        if (Input::IsMouseKeyDown(VELT_MOUSE2) && !Input::IsKeyDown(Scancode::VELT_SCANCODE_LALT))
        {
            m_CameraMode = CameraMode::Flycam;
            DisableMouse();

            const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
            const float speed = GetCameraSpeed();

            if (Input::IsKeyDown(Scancode::VELT_SCANCODE_Q))
                m_PositionDelta -= ts.GetMilliseconds() * speed * glm::vec3{ 0.0f, yawSign, 0.0f };
            if (Input::IsKeyDown(Scancode::VELT_SCANCODE_E))
                m_PositionDelta += ts.GetMilliseconds() * speed * glm::vec3{ 0.0f, yawSign, 0.0f };
            if (Input::IsKeyDown(Scancode::VELT_SCANCODE_S))
                m_PositionDelta -= ts.GetMilliseconds() * speed * m_Direction;
            if (Input::IsKeyDown(Scancode::VELT_SCANCODE_W))
                m_PositionDelta += ts.GetMilliseconds() * speed * m_Direction;
            if (Input::IsKeyDown(Scancode::VELT_SCANCODE_A))
                m_PositionDelta -= ts.GetMilliseconds() * speed * m_RightDirection;
            if (Input::IsKeyDown(Scancode::VELT_SCANCODE_D))
                m_PositionDelta += ts.GetMilliseconds() * speed * m_RightDirection;

            constexpr float maxRate = 0.12f;
            m_YawDelta += glm::clamp(yawSign * delta.x * RotationSpeed(), -maxRate, maxRate);
            m_PitchDelta += glm::clamp(delta.y * RotationSpeed(), -maxRate, maxRate);

            m_RightDirection = glm::cross(m_Direction, glm::vec3{ 0.0f, yawSign, 0.0f });
            m_Direction = -glm::rotate(
                glm::normalize(glm::cross(
                    glm::angleAxis(-m_PitchDelta, m_RightDirection),
                    glm::angleAxis(-m_YawDelta, glm::vec3{ 0.0f, yawSign, 0.0f })
                )),
                m_Direction
            );

            const float distance = glm::distance(m_FocalPoint, m_Position);
            m_FocalPoint = m_Position + GetForwardDirection() * distance;
            m_Distance = distance;
        }
        else if (Input::IsKeyDown(Scancode::VELT_SCANCODE_LALT))
        {
            m_CameraMode = CameraMode::Arcball;

            if (Input::IsMouseKeyDown(VELT_MOUSE3))
            {
                DisableMouse();
                MousePan(delta);
            }
            else if (Input::IsMouseKeyDown(VELT_MOUSE1))
            {
                DisableMouse();
                MouseRotate(delta);
            }
            else if (Input::IsMouseKeyDown(VELT_MOUSE2))
            {
                DisableMouse();
                MouseZoom((delta.x + delta.y) * 0.1f);
            }
            else
            {
                EnableMouse();
            }
        }
        else
        {
            EnableMouse();
        }

        m_InitialMousePosition = mouse;
        m_Position += m_PositionDelta;
        m_Yaw += m_YawDelta;
        m_Pitch += m_PitchDelta;

        if (m_CameraMode == CameraMode::Arcball)
            m_Position = CalculatePosition();

        UpdateView();
    }

    float EditorCamera::GetCameraSpeed() const
    {
        float speed = m_NormalSpeed;
        if (Input::IsKeyDown(Scancode::VELT_SCANCODE_LCTRL))
            speed /= 2.0f - glm::log(m_NormalSpeed);
        if (Input::IsKeyDown(Scancode::VELT_SCANCODE_LSHIFT))
            speed *= 2.0f - glm::log(m_NormalSpeed);

        return glm::clamp(speed, MIN_SPEED, MAX_SPEED);
    }

    void EditorCamera::UpdateView()
    {
        VT_PROFILE_FUNCTION();
        const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;

        const float cosAngle = glm::dot(GetForwardDirection(), GetUpDirection());
        if (cosAngle * yawSign > 0.99f)
            m_PitchDelta = 0.0f;

        const glm::vec3 lookAt = m_Position + GetForwardDirection();
        m_Direction = glm::normalize(lookAt - m_Position);
        m_Distance = glm::distance(m_Position, m_FocalPoint);
        m_ViewMatrix = glm::lookAt(m_Position, lookAt, glm::vec3{ 0.0f, yawSign, 0.0f });

        m_YawDelta *= 0.6f;
        m_PitchDelta *= 0.6f;
        m_PositionDelta *= 0.8f;
    }

    void EditorCamera::Focus(const Vector& focusPoint)
    {
        m_FocalPoint = focusPoint;
        m_CameraMode = CameraMode::Flycam;
        if (m_Distance > m_MinFocusDistance)
        {
            m_Distance -= m_Distance - m_MinFocusDistance;
            m_Position = m_FocalPoint - GetForwardDirection() * m_Distance;
        }

        m_Position = m_FocalPoint - GetForwardDirection() * m_Distance;
        UpdateView();
    }

    std::pair<float, float> EditorCamera::PanSpeed() const
    {
        const float x = glm::min(m_ViewportWidth / 1000.0f, 2.4f);
        const float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        const float y = glm::min(m_ViewportHeight / 1000.0f, 2.4f);
        const float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { -xFactor * 0.01, -yFactor * 0.01 };
    }

    float EditorCamera::RotationSpeed() const
    {
        return 0.3f;
    }

    float EditorCamera::ZoomSpeed() const
    {
        float distance = m_Distance * 0.2f;
        distance = glm::max(distance, 0.0f);
        float speed = distance * distance;
        speed = glm::min(speed, 50.0f);
        return speed;
    }

    Vector EditorCamera::GetUpDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    Vector EditorCamera::GetRightDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    Vector EditorCamera::GetForwardDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    Quaternion EditorCamera::GetOrientation() const
    {
        return glm::quat(glm::vec3(-m_Pitch - m_PitchDelta, -m_Yaw - m_YawDelta, 0.0f));
    }

    void EditorCamera::OnEvent(Event& e)
    {
        VT_PROFILE_FUNCTION();
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& event) { return OnMouseScroll(event); });
    }

    bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
    {
        if (Input::IsMouseKeyDown(VELT_MOUSE2))
        {
            m_NormalSpeed += e.GetYOffset() * 0.3f * m_NormalSpeed;
            m_NormalSpeed = std::clamp(m_NormalSpeed, MIN_SPEED, MAX_SPEED);
        }
        else
        {
            MouseZoom(e.GetYOffset() * 0.1f);
            UpdateView();
        }

        return true;
    }

    void EditorCamera::MousePan(const glm::vec2& delta)
    {
        auto [xSpeed, ySpeed] = PanSpeed();
        m_FocalPoint += GetRightDirection() * delta.x * xSpeed * m_Distance;
        m_FocalPoint -= GetUpDirection() * delta.y * ySpeed * m_Distance;
    }

    void EditorCamera::MouseRotate(const glm::vec2& delta)
    {
        const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
        m_YawDelta += yawSign * delta.x * RotationSpeed();
        m_PitchDelta += delta.y * RotationSpeed();
    }

    void EditorCamera::MouseZoom(float delta)
    {
        m_Distance -= delta * ZoomSpeed();
        const glm::vec3 forwardDir = GetForwardDirection();
        m_Position = m_FocalPoint - forwardDir * m_Distance;

        if (m_Distance < 1.0f)
        {
            m_FocalPoint += forwardDir * m_Distance;
            m_Distance = 1.0f;
        }

        m_PositionDelta += delta * ZoomSpeed() * forwardDir;
    }

    Vector EditorCamera::CalculatePosition() const
    {
        return m_FocalPoint - GetForwardDirection() * m_Distance + m_PositionDelta;
    }
}