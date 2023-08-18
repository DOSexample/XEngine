#include <X/Engine/GameObject.h>
#include <X/Engine/Object.h>

#include <X/Engine/Component/Transform.h>

namespace X {


	void AddNewGameObject(GameObject* go)
	{
		auto com = std::make_shared<Transform>();
		go->transform = com.get();
		go->gameObject = go;

		com->gameObject = go->gameObject;
		com->transform = go->transform;
		go->AddComponent(com);
	}

    GameObject::GameObject()
	{
		DebugA( "New GameObject\r\n" );
		AddNewGameObject(this);
	}

	GameObject::~GameObject()
	{
	}
}