#pragma once

#include <vector>

#include <X/Engine/Component.h>

#include <d3dx9.h>

namespace X {

	class Graphic;

	class DXMesh : public Component
	{
	public:
		DXMesh();
		~DXMesh();

		void Draw();

		static EXOBJECT_TYPE ConstType()
		{
			return XOBJ_DXMESH;
		}

	private:
		ID3DXMesh* jointMesh = nullptr;
		ID3DXLine* pLine = nullptr;

		float jointRadius = 0.05f;  // Radius of the joint
		int slices = 20, stacks = 20;  // Quality of the sphere

	};

}