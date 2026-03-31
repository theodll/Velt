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
		Camera(const Matrix& projection)
			: m_Projection(projection) {
		}

		virtual ~Camera() = default;
			
		const Matrix& GetProjection() const { return m_Projection; }
		const Matrix& GetViewProjection() const { return m_ViewProjection; }
		const Matrix& GetInverseViewProjection() const { return glm::inverse(m_ViewProjection); }
		const Vector& GetPosition() const { return m_Position; }
	protected:
		Matrix m_Projection = Matrix(1.0f);
		Matrix m_ViewMatrix = Matrix(1.0f);
		Matrix m_ViewProjection = Matrix(1.0f);
		Vector m_Position{ 0.0f, 0.0f, 5.0f };

	};

	/*
	class Camera O
	{
	public:
		Camera() = default; 
        virtual ~Camera() = default;

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
	};*/
}