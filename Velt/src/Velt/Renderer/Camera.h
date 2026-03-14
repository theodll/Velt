#pragma once
#include "Core/Core.h"
#include "Core/Math.h"
#include <glm/glm.hpp>

namespace Velt 
{
	class Camera 
	{
	public:
		Camera() = default; 

		void SetOrthographicProjection(float left, float right, float top, float bottom, float near, float far); 
		void SetPerspectiveProjection(float fovY, float aspect, float near, float far);

		const Matrix& GetProjectionMatrix() const { return m_ProjectionMatrix; };
	private: 
		Matrix m_ProjectionMatrix{ 1.0f };
	};
}