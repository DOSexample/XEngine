#pragma once

#include <d3dx9.h>

#include <string>

namespace X {

	class Graphic;
	class GameObject;
	class Transform;

	enum EXOBJECT_TYPE : int
	{
		XOBJ_UNKNOW = 0,
		XOBJ_BASE_OBJECT,
		XOBJ_BASE_COMPONENT,
		XOBJ_TRANSFORM,
		XOBJ_CAMERA,
		XOBJ_GAME_OBJECT,
		XOBJ_MESH,
		XOBJ_DXMESH
	};

	class Object
	{
	public:
		Object();
		~Object() = default;

		virtual void Awake();
		virtual void Start();
		virtual void FixedUpdate();
		virtual void Update();
		virtual void LateUpdate();
		virtual void OnEnable();
		virtual void OnDisable();

		virtual void Draw();

	public:
		unsigned int hash;
		std::string m_Name;
		EXOBJECT_TYPE m_Type;

		GameObject* gameObject;
		Transform* transform;

		static EXOBJECT_TYPE ConstType()
		{
			return XOBJ_BASE_OBJECT;
		}
	};

}