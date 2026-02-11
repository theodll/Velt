#include "vtpch.h"
#include "OrthographicCamera.h"

namespace Velt {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top)
	{
		RecalculateProjection();
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_Left = left; m_Right = right; m_Bottom = bottom; m_Top = top;
		RecalculateProjection();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateProjection()
	{
		m_ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_Near, m_Far);
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -1.0f)); 

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
