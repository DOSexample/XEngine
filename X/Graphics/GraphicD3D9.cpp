#include <X/Graphics/GraphicD3D9.h>
#include <X/Graphics/Graphic.h>

#include <X/Core/Win32Native.h>

namespace X {
	
    GraphicD3D9::GraphicD3D9(Win32Native* win32native)
	{
        D3DPRESENT_PARAMETERS d3dpp = {};
        d3dpp.BackBufferWidth = win32native->GetWidth();
        d3dpp.BackBufferHeight = win32native->GetHeight();
        d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
        d3dpp.BackBufferCount = 1;
        d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
        d3dpp.MultiSampleQuality = 0;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.hDeviceWindow = win32native->GetHWND();
        d3dpp.Windowed = TRUE;
        d3dpp.EnableAutoDepthStencil = TRUE;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
        d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
        d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_DEFAULT;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        m_pDevice = nullptr;
        m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, win32native->GetHWND(),
            //D3DCREATE_SOFTWARE_VERTEXPROCESSING
            D3DCREATE_HARDWARE_VERTEXPROCESSING
            , &d3dpp, &m_pDevice);
	}

    GraphicD3D9::~GraphicD3D9()
    {
    }

    void GraphicD3D9::Release()
    {
        SafeRelease( m_pDevice );
        SafeRelease( m_pD3D );
    }

    HRESULT GraphicD3D9::Clear( DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil )
    {
        return m_pDevice->Clear( Count, pRects, Flags, Color, Z, Stencil ); 
    }

    void GraphicD3D9::BeginScene(D3DCOLOR color)
    {
        Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, color, 1.0f, 0);
        m_pDevice->BeginScene();
    }

    void GraphicD3D9::EndScene()
    {
        m_pDevice->EndScene();
        m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
    }
}