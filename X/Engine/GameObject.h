#pragma once

#include <X/Engine/Object.h>

#include <memory>
#include <vector>
#include <string>

#include <X/Core/Debug.h>

#include <X/Engine/Component/Camera.h>
#include <X/Engine/Component/DXMesh.h>
#include <X/Engine/Component/Transform.h>

namespace X {

	class Component;
	class Camera;
	class Transform;
	class DXMesh;
	class GameObject : public Object
	{
	public:
		GameObject();
		~GameObject();

		template<typename T> T* AddComponent();
		template<typename T> T* AddComponent(std::shared_ptr<T> com);

		template<typename T> T* GetComponent();
		template<typename T> std::vector< T* > GetComponents();

		std::vector< std::shared_ptr<Component> > components;

		static EXOBJECT_TYPE ConstType()
		{
			return XOBJ_GAME_OBJECT;
		}
	};

	template<typename T> inline T* GameObject::AddComponent()
	{
		DebugA( "AddComponent: %s \r\n", typeid(T).name() );
		return AddComponent( std::make_shared<T>() );
	}

	template<typename T> inline T* GameObject::AddComponent(std::shared_ptr<T> com)
	{
		DebugA( "AddComponent2: %s \r\n", typeid(T).name() );

		com->gameObject = gameObject;
		com->transform = transform;
		components.push_back(com);

		return com.get();
	}

	template<typename T> inline T* GameObject::GetComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "Component only");
		std::vector< T* > results = GetComponents<T>();

		if (results.size() < 1) return nullptr;

		return (T*)results[0];
	}

	template<typename T> inline std::vector< T* > GameObject::GetComponents()
	{
		static_assert(std::is_base_of<Component, T>::value, "Component only");

		std::vector< T* > out_components;

		int i = 0;
		for (auto comp : components)
		{
			auto type = comp->m_Type;
			if ( type != XOBJ_UNKNOW && type == T::ConstType() )
			{
				out_components.push_back( ((T*)comp.get()) );
			}

			i++;
		}

		DebugA("%s >> result size = %d\r\n", typeid(T).name(), out_components.size());

		return out_components;
	}
}