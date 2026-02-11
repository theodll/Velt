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
        glm::vec3 translation{ 0.0f };
        glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f }; // identity (w,x,y,z)
        glm::vec3 scale{ 1.0f };

        // only for editor
        glm::vec3 eulerDegrees{ 0.0f };

        void SetEulerDegrees(const glm::vec3& deg)
        {
            eulerDegrees = deg;
            rotation = glm::quat(glm::radians(deg)); 
            rotation = glm::normalize(rotation);
        }

        glm::mat4 mat4() const
        {
            return glm::translate(glm::mat4(1.0f), translation)
                * glm::toMat4(rotation)
                * glm::scale(glm::mat4(1.0f), scale);
        }
    };
}
