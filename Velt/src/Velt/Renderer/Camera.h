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
        virtual ~Camera();

		void SetOrthographicProjection(float left, float right, float top, float bottom, float near, float far); 
		void SetPerspectiveProjection(float fovY, float aspect, float near, float far);

        void SetViewDirection(Vector position, Vector direction, Vector up = Vector(0.0f, -1.0f, 0.0f));
        void SetViewTarget(Vector position, Vector target, Vector up = Vector(0.0f, -1.0f, 0.0f));

        void SetViewQuat(Vector position, Quaternion quaternion);
        void SetViewYXZ(Vector position, EAngle euler);

		const Matrix& GetProjectionMatrix() const { return m_ProjectionMatrix; };
        const Matrix& GetViewMatrix() const { return m_ViewMatrix; }
	private: 
		Matrix m_ProjectionMatrix{ 1.0f };
        Matrix m_ViewMatrix{1.0f};  
	};
}