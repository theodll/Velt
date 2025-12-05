#pragma once
#include <vtpch.h>

#include "Model.h"


namespace Velt
{
	struct TransformComponent
	{
		glm::vec2 translation{}; 

		glm::mat2 mat2() {}
	};

	class GameObject 
	{
	public:

		static GameObject createGameObject()
		{
			static uint32_t currentID = 0;
			return GameObject{ currentID++ };
		}

		GameObject(const GameObject &) = delete;
		GameObject &operator=(const GameObject &) = delete;
		GameObject(GameObject &&) = default;
		GameObject& operator=(GameObject &&) = default;


		uint32_t getID() const { return id; }

		std::shared_ptr<RenderAPI::Model> model;
		glm::vec3 color; 

	private:
		GameObject(uint32_t objInt) : id{ objInt } {};
		
		uint32_t id;
	};
}