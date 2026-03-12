#pragma once
#include <glm/glm.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>   
#include <glm/gtc/quaternion.hpp>         
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>  



namespace Velt 
{
	using HVector = glm::vec4; 
	using Vector = glm::vec3;
	using Point = glm::vec2;
	using Number = glm::vec1;

	using Matrix = glm::mat4;

	using Quaternion = glm::quat;
	using EAngle = glm::vec3;
}