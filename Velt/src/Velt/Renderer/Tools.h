#pragma once
#include "Velt/Core/Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Velt::Renderer 
{
    struct TransformComponent
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        // Note [07.02.26]: This Matrix corresponds to: translate * rotateY * rotateX * rotateZ * scale in this specific order.

        glm::mat4 mat4() 
        {
            auto transform = glm::translate(glm::mat4(1.0f), position);

            transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

            transform = glm::scale(transform, scale);
            return transform;
        }

        TransformComponent()
            : position(0.0f), rotation(0.0f), scale(1.0f) {}
    };
}