#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "Velt/Core/Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Velt 
{

    // Todo [08.02.26, Theo]: Move this to some component / ecs implementation
    struct TransformComponent
    {
        Vector Translation{ 0.0f };
        Quaternion Rotation{ 1.0f, 0.0f, 0.0f, 0.0f }; // identity (w,x,y,z)
        Vector Scale{ 1.0f };

        // only for editor
        Vector EulerDegrees{ 0.0f };

        void SetEulerDegrees(const Vector& deg)
        {
            EulerDegrees = deg;
            Rotation = glm::quat(glm::radians(deg));
            Rotation = glm::normalize(Rotation);
        }

        Matrix Matrix() const
        {
            return glm::translate(glm::mat4(1.0f), Translation)
                * glm::toMat4(Rotation)
                * glm::scale(glm::mat4(1.0f), Scale);
        }
    };
}
