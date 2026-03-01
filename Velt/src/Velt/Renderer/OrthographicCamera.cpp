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
		Matrix transform =
			glm::translate(Matrix(1.0f), m_Position) *
			glm::rotate(Matrix(1.0f), glm::radians(m_Rotation), Vector(0, 0, 1)) *
			glm::scale(Matrix(1.0f), Vector(1.0f, 1.0f, -1.0f)); 

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
