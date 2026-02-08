#pragma once
#include <glm/glm.hpp>

namespace Velt {

	class OrthographicCamera
	{
	public:
		OrthographicCamera() : OrthographicCamera(-1.0f, 1.0f, -1.0f, 1.0f) {}
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }              // in degrees
		void SetRotation(float rotationDegrees) { m_Rotation = rotationDegrees; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();
		void RecalculateProjection();

	private:
		glm::mat4 m_ProjectionMatrix{ 1.0f };
		glm::mat4 m_ViewMatrix{ 1.0f };
		glm::mat4 m_ViewProjectionMatrix{ 1.0f };

		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f; // degrees

		float m_Left = -1.0f, m_Right = 1.0f, m_Bottom = -1.0f, m_Top = 1.0f;
		float m_Near = 0.0f, m_Far = 10.0f; 
	};

}
