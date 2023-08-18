#pragma once

#include <vector>

namespace X {

	class GameObject;

	class Scene
	{
	public:
		Scene();
		~Scene() = default;

		void AddGameObject(GameObject* obj);

		void Update();


		std::vector<GameObject*> m_gameObjects;

	};

}