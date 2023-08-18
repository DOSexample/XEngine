#include <X/Engine/Component.h>

#include <X/Engine/GameObject.h>
#include <X/Engine/Component/Transform.h>

namespace X {

	Component::Component() : Object(), gameObject(nullptr), transform(nullptr)
	{
	}

    Component::~Component() {
    }
}