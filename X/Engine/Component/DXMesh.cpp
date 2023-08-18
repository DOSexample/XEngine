#include <X/Engine/Component/DXMesh.h>

#include <X/Graphics/Graphic.h>

#include <X/Engine/Component/Transform.h>

#include <X/Core/Debug.h>

namespace X {

	struct Joint {
		D3DXVECTOR3 position;  // The position of the joint in 3D space
	};

	struct Bone {
		Joint* startJoint;   // Pointer to the starting joint
		Joint* endJoint;     // Pointer to the ending joint
	};

    DXMesh::DXMesh() : Component()
	{
		DebugA("%s\r\n",__FUNCTION__);
		m_Type = XOBJ_DXMESH;
	}

    DXMesh::~DXMesh() {
    }

	void Bind()
	{
	}

	void DXMesh::Draw()
	{
		auto device = gfx->GetDevice();

		// Vertex Data
		struct Vertex {
			float x, y, z;
		};

		Vertex vertices[] = {
			{ 0.0f, 0.5f, 0.0f },
			{ 0.5f, -0.5f, 0.0f },
			{ -0.5f, -0.5f, 0.0f }
		};

		static IDirect3DVertexBuffer9* pVertexBuffer;
		if (!pVertexBuffer) {
			device->CreateVertexBuffer(3 * sizeof(Vertex), 0, D3DFVF_XYZ, D3DPOOL_DEFAULT, &pVertexBuffer, NULL);
			void* pVoid;
			pVertexBuffer->Lock(0, 0, &pVoid, 0);
			memcpy(pVoid, vertices, sizeof(vertices));
			pVertexBuffer->Unlock();
		}

		device->SetStreamSource(0, pVertexBuffer, 0, sizeof(Vertex));
		device->SetFVF(D3DFVF_XYZ);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		if (transform)
		{
			auto device = gfx->GetDevice();

			D3DXMATRIX mtx = transform->GetWorldMatrix();

			device->SetTransform(D3DTS_WORLD, &mtx);
		}
	}
}