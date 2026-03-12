#pragma once
#include "Core/Core.h"
#include "Core/Math.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace Velt
{

	class Camera
	{
	public:
		Camera() = default;
		Camera(const Matrix& projection, const Matrix& unReversedProjection);
		Camera(const float degFov, const float width, const float height, const float nearP, const float farP);
		virtual ~Camera() = default;

		const Matrix& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const Matrix& GetUnReversedProjectionMatrix() const { return m_UnReversedProjectionMatrix; }

		void SetProjectionMatrix(const Matrix projection, const Matrix unReversedProjection)
		{
			m_ProjectionMatrix = projection;
			m_UnReversedProjectionMatrix = unReversedProjection;
		}

		void SetPerspectiveProjectionMatrix(const float radFov, const float width, const float height, const float nearP, const float farP)
		{
			m_ProjectionMatrix = glm::perspectiveFov(radFov, width, height, farP, nearP);
			m_UnReversedProjectionMatrix = glm::perspectiveFov(radFov, width, height, nearP, farP);
		}

		void SetOrthoProjectionMatrix(const float width, const float height, const float nearP, const float farP)
		{
			m_ProjectionMatrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, farP, nearP);
			m_UnReversedProjectionMatrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, nearP, farP);
		}

		float GetExposure() const { return m_Exposure; }
		float& GetExposure() { return m_Exposure; }
	protected:
		float m_Exposure = 0.8f;
	private:
		Matrix m_ProjectionMatrix = glm::mat4(1.0f);
		Matrix m_UnReversedProjectionMatrix = glm::mat4(1.0f);
	};

}