#include "vtpch.h"
#include "Core/Core.h"
#include "Core/Math.h"
#include "Camera.h"
/*
namespace Velt 
{
	 void Camera::SetOrthographicProjection(float left, float right, float top, float bottom, float near, float far) 
	 {
		 m_ProjectionMatrix = glm::mat4{ 1.0f };
		 m_ProjectionMatrix[0][0] = 2.0f / (right - left);
		 m_ProjectionMatrix[1][1] = 2.0f / (bottom - top);
		 m_ProjectionMatrix[2][2] = 1.0f / (far - near);
		 m_ProjectionMatrix[3][0] = -(right + left) / (right - left);
		 m_ProjectionMatrix[3][1] = -(bottom + top) / (bottom - top);
		 m_ProjectionMatrix[3][2] = -near / (far - near);
	 }

	 void Camera::SetPerspectiveProjection(float fovY, float aspect, float near, float far) 
	 {
		 VT_CORE_ASSERT(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f, "");
		 const float tanHalfFovY = tan(fovY / 2.0f);
		 m_ProjectionMatrix = glm::mat4{ 0.0f };
		 m_ProjectionMatrix[0][0] = 1.0f / (aspect * tanHalfFovY);
		 m_ProjectionMatrix[1][1] = 1.0f / (tanHalfFovY);
		 m_ProjectionMatrix[2][2] = far / (far - near);
		 m_ProjectionMatrix[2][3] = 1.0f;
		 m_ProjectionMatrix[3][2] = -(far * near) / (far - near);
	 }

     void Camera::SetViewQuat(Vector position, Quaternion quaternion) 
     {
        m_ViewMatrix =
        glm::mat4_cast(glm::conjugate(quaternion)) *
        glm::translate(glm::mat4(1.f), -position);
     }

     void Camera::SetViewYXZ(Vector position, EAngle euler) 
     {
        Quaternion quat = Quaternion(euler);
        SetViewQuat(position, quat);
     }

     void Camera::SetViewDirection(Vector position, Vector direction, Vector up) {
        const glm::vec3 w{glm::normalize(direction)};
        const glm::vec3 u{glm::normalize(glm::cross(w, up))};
        const glm::vec3 v{glm::cross(w, u)};

        m_ViewMatrix = glm::mat4{1.f};
        m_ViewMatrix[0][0] = u.x;
        m_ViewMatrix[1][0] = u.y;
        m_ViewMatrix[2][0] = u.z;
        m_ViewMatrix[0][1] = v.x;
        m_ViewMatrix[1][1] = v.y;
        m_ViewMatrix[2][1] = v.z;
        m_ViewMatrix[0][2] = w.x;
        m_ViewMatrix[1][2] = w.y;
        m_ViewMatrix[2][2] = w.z;
        m_ViewMatrix[3][0] = -glm::dot(u, position);
        m_ViewMatrix[3][1] = -glm::dot(v, position);
        m_ViewMatrix[3][2] = -glm::dot(w, position);
    }

    void Camera::SetViewTarget(Vector position, Vector target, Vector up) {
        SetViewDirection(position, target - position, up);
    }

} */