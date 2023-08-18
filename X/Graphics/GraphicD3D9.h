#pragma once

#include <d3d9.h>
#include <d3dx9.h>

namespace X {

	class Win32Native;

	class GraphicD3D9
	{
	public:
		GraphicD3D9(Win32Native* win32native);
		~GraphicD3D9();
		void Release();

		HRESULT Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);

		void BeginScene(D3DCOLOR color);
		void EndScene();

		IDirect3DDevice9* GetDevice() { return m_pDevice; }

	private:
		IDirect3D9* m_pD3D;
		IDirect3DDevice9* m_pDevice;
	};

}
