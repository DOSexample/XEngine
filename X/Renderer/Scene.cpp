#include <X/Renderer/Scene.h>

#include <X/Graphics/Graphic.h>

#include <X/Engine/Object.h>
#include <X/Engine/GameObject.h>
#include <X/Engine/Timer.h>

#include <X/Engine/Component/Camera.h>
#include <X/Engine/Component/Transform.h>

#include <functional>

namespace X {

	Scene::Scene()
	{}

	void Scene::AddGameObject(GameObject* go)
	{
		m_gameObjects.push_back( go );
	}

	void Scene::Update()
	{
		for (auto obj : m_gameObjects) {
			for (auto com : obj->components)
			{
				auto get = com.get();
				auto type = ((Object*)get)->m_Type;
				switch (type)
				{
				case XOBJ_TRANSFORM:
					((Transform*)get)->FixedUpdate();
					break;
				case XOBJ_CAMERA:
					((Camera*)get)->FixedUpdate();
					break;
				default:
					obj->FixedUpdate();
					break;
				}
			}
			obj->FixedUpdate();
		}

		for (auto obj : m_gameObjects) {
			for (auto com : obj->components)
			{
				auto get = com.get();
				auto type = ((Object*)get)->m_Type;
				switch ( type )
				{
				case XOBJ_TRANSFORM:
					((Transform*)get)->Update();
					break;
				case XOBJ_CAMERA:
					((Camera*)get)->Update();
					break;
				default:
					obj->Update();
					break;
				}
			}
			obj->Update();
		}

		for (auto obj : m_gameObjects) {
			for (auto com : obj->components)
			{
				auto get = com.get();
				auto type = ((Object*)get)->m_Type;
				switch (type)
				{
				case XOBJ_TRANSFORM:
					((Transform*)get)->LateUpdate();
					break;
				case XOBJ_CAMERA:
					((Camera*)get)->LateUpdate();
					break;
				default:
					obj->LateUpdate();
					break;
				}
			}
			obj->LateUpdate();
		}

	}
}