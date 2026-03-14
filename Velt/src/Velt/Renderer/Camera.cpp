#include "vtpch.h"
#include "Core/Core.h"
#include "Core/Math.h"
#include "Camera.h"
#include "math.h"

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
}