#pragma once

#include <X/Engine/Object.h>

namespace X {

	class Object;
	class Transform;
	class GameObject;

	class Component : public Object
	{
	public:
		Component();
		~Component();

	public:
		Transform* transform;
		GameObject* gameObject;

		static EXOBJECT_TYPE ConstType()
		{
			return XOBJ_BASE_COMPONENT;
		}
	};

}