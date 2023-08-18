#pragma once

#include <vector>

#include <X/Engine/Component.h>

namespace X {

	class Mesh : public Component
	{
	public:
		Mesh();
		~Mesh();

	public:
		std::vector<float> vertices;
		std::vector<float> normal;
		std::vector<float> texcoord;

		static EXOBJECT_TYPE ConstType()
		{
			return XOBJ_MESH;
		}

	};

}