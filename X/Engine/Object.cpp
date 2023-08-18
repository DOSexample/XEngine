#include <X/Engine/Object.h>
#include <X/Engine/Component/Transform.h>

#include <X/Core/Win32Native.h>

#include <X/Graphics/Graphic.h>

#include <string>

namespace X {

	static int s_object_id = 0;
	static int s_counting = 0;
	Object::Object() : 
		hash(0),
		gameObject(nullptr),
		transform(nullptr),
		m_Type(XOBJ_UNKNOW),
		m_Name("")
	{
	}

	void Object::Awake()
	{
	}

	void Object::Start()
	{
	}

	void Object::FixedUpdate()
	{
	}

	void Object::Update()
	{
	}

	void Object::LateUpdate()
	{
	}

	void Object::OnEnable()
	{
	}

	void Object::OnDisable()
	{
	}

	void Object::Draw()
	{
	}

}