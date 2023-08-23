#include "XDevice.h"

#include "XCamera.h"
#include "XTexture.h"

LPDIRECT3D9 m_D3D = nullptr;
LPDIRECT3DDEVICE9 m_pDevice = nullptr;
D3DCAPS9 m_Caps;

IDirect3DVertexDeclaration9* mDECLForSKIN = nullptr;
IDirect3DVertexDeclaration9* mDECLForSKIN2 = nullptr;
IDirect3DVertexDeclaration9* mDECLForSKIN2SHADOW = nullptr;
D3DVERTEXELEMENT9 mVertexElementForSKIN[] = {
    {0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,           0},
    {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,             0},
    {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,           0},
    D3DDECL_END()
};
D3DVERTEXELEMENT9 mVertexElementForSKIN2[] = {
    {0,  0, D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,       0},
    {0, 12, D3DDECLTYPE_FLOAT4,     D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,    0},
    {0, 28, D3DDECLTYPE_D3DCOLOR,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,   0},
    {0, 32, D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,        0},
    {0, 44, D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,       0},
    {0, 56, D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,         0},
    {0, 68, D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,       0},
    D3DDECL_END()
};
D3DVERTEXELEMENT9 mVertexElementForSKIN2SHADOW[] = {
    {0,  0, D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,       0},
    {0, 12, D3DDECLTYPE_FLOAT4,     D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,    0},
    {0, 28, D3DDECLTYPE_D3DCOLOR,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,   0},
    D3DDECL_END()
};

D3DXMATRIX mWorldMatrix;
XCamera* mMainCamera = nullptr;
D3DMATERIAL9 mMaterial;
D3DLIGHT9 mLight;

XDevice::XDevice()
{
}

XDevice::~XDevice()
{
    SAFE_RELEASE( mDECLForSKIN2 );
    SAFE_RELEASE( m_pDevice );
    SAFE_RELEASE( m_D3D );
}

void* XDevice::GetRealDevice()
{
    return m_pDevice;
}

bool XDevice::Initialize(void* hWnd, int width, int height)
{
    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = (HWND)hWnd;
    d3dpp.Windowed = TRUE;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    HRESULT hr;
    m_pDevice = nullptr;

    m_D3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!m_D3D)
        return false;

    DWORD BehaviorFlags = 0;
    //BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    BehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

    hr = m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWnd, BehaviorFlags, &d3dpp, &m_pDevice);
    if (FAILED(hr))
        return false;

    if ( FAILED( m_D3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_Caps ) ) )
        return false;

    if ( 
        FAILED( m_D3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT1 ) ) || 
        FAILED( m_D3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT3 ) ) || 
        FAILED( m_D3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT5 ) )
        )
        return false;

    if (m_pDevice)
    {
        memset( &mMaterial, 0, sizeof( mMaterial) );
        mMaterial.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
        mMaterial.Ambient = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
        mMaterial.Specular = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
        mMaterial.Emissive = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
        mMaterial.Power = 0.0f;
        m_pDevice->SetMaterial(&mMaterial);

        m_pDevice->LightEnable(0,1);
        memset( &mLight, 0, sizeof( mLight) );
        mLight.Type = D3DLIGHT_DIRECTIONAL;
        mLight.Diffuse = D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.0f );
        mLight.Specular = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
        mLight.Ambient = D3DXCOLOR( 0.4f, 0.4f, 0.4f, 1.0f );
        mLight.Position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
        mLight.Direction = D3DXVECTOR3( -1.0f, -1.0f, 1.0f );
        D3DXVec3Normalize( (D3DXVECTOR3*)&mLight.Direction, (D3DXVECTOR3*)&mLight.Direction );
        mLight.Range = 0.0f;
        mLight.Falloff = 0.0f;
        mLight.Attenuation0 = 0.0f;
        mLight.Attenuation1 = 0.0f;
        mLight.Attenuation2 = 0.0f;
        mLight.Theta = 0.0f;
        mLight.Phi = 0.0f;
        m_pDevice->SetLight( 0, &mLight );
    }

    return CreateVertexDeclaration();
}

void XDevice::Clear(ClearType clearType, ColorType colorType, float z, DWORD Stencil)
{
    m_pDevice->Clear(0, NULL, clearType, colorType, z, Stencil);
}

void XDevice::BeginScene()
{
    m_pDevice->BeginScene();
    D3DXMatrixIdentity( &mWorldMatrix );
    m_pDevice->SetTransform( D3DTS_WORLD, &mWorldMatrix );

    {
        m_pDevice->SetMaterial(&mMaterial);
        m_pDevice->LightEnable(0, TRUE);
        m_pDevice->SetLight(0, &mLight);
    }
    {   //Sampler : Texture
        m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        m_pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        m_pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        m_pDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
        m_pDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        m_pDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        m_pDevice->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        m_pDevice->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        m_pDevice->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        m_pDevice->SetSamplerState(2, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        m_pDevice->SetSamplerState(2, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    }
    //{   //Render State (Vertex,Color,Alpha-Color,ZWrite Depth, Etc)
    //    m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    //    m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    //    m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    //    m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        m_pDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
    //}
}

void XDevice::EndScene()
{
    m_pDevice->EndScene();
    m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

D3DXMATRIX XDevice::GetWorldMatrix()
{
    return mWorldMatrix;
}

D3DXMATRIX XDevice::GetViewMatrix()
{
    return *mMainCamera->GetViewMatrix();
}

D3DXMATRIX XDevice::GetPerspectiveMatrix()
{
    return *mMainCamera->GetPerspectiveMatrix();
}

void XDevice::SetCamera(XCamera* cam)
{
    mMainCamera = cam;
}

const D3DLIGHT9 XDevice::GetDefaultLight()
{
    return mLight;
}

BOOL XDevice::CreateVertexDeclaration()
{
    return SUCCEEDED( m_pDevice->CreateVertexDeclaration( mVertexElementForSKIN2, &mDECLForSKIN2 ) );
}

BOOL XDevice::SetVertexDeclaration()
{
    return SUCCEEDED( m_pDevice->SetVertexDeclaration( mDECLForSKIN2 ) );
}

BOOL XDevice::CreateTexture(
    LPCVOID                   pSrcData,
    UINT                      SrcDataSize,
    UINT                      Width,
    UINT                      Height,
    UINT                      MipLevels,
    DWORD                     Usage,
    D3DFORMAT                 Format,
    D3DPOOL                   Pool,
    DWORD                     Filter,
    DWORD                     MipFilter,
    D3DCOLOR                  ColorKey,
    D3DXIMAGE_INFO*           pSrcInfo,
    PALETTEENTRY*             pPalette,
    LPDIRECT3DTEXTURE9*       ppTexture
)
{
    return SUCCEEDED( D3DXCreateTextureFromFileInMemoryEx(m_pDevice, pSrcData, SrcDataSize, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture ) );
}



void XDevice::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
    m_pDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}
void XDevice::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
    m_pDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

void XDevice::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    m_pDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

void XDevice::DrawIndexedPrimitive(THIS_ D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
    m_pDevice->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

void XDevice::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    m_pDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

void XDevice::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    m_pDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

void XDevice::SetStreamSource( UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
    m_pDevice->SetStreamSource( StreamNumber, pStreamData, OffsetInBytes, Stride );
}

void XDevice::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
{
    m_pDevice->GetStreamSource( StreamNumber, ppStreamData, pOffsetInBytes, pStride );
}

void XDevice::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
    m_pDevice->SetIndices(pIndexData);
}
void XDevice::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
    m_pDevice->GetIndices(ppIndexData);
}

void XDevice::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
    m_pDevice->SetTransform(State, pMatrix);
}


HRESULT XDevice::TestCooperativeLevel()
{
    return m_pDevice->TestCooperativeLevel();
}

UINT XDevice::GetAvailableTextureMem()
{
    return m_pDevice->GetAvailableTextureMem();
}

HRESULT XDevice::EvictManagedResources()
{
    return m_pDevice->EvictManagedResources();
}

HRESULT XDevice::GetDirect3D(IDirect3D9** ppD3D9)
{
    return m_pDevice->GetDirect3D(ppD3D9);
}

HRESULT XDevice::GetDeviceCaps(D3DCAPS9* pCaps)
{
    return m_pDevice->GetDeviceCaps(pCaps);
}

HRESULT XDevice::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
    return m_pDevice->GetDisplayMode( iSwapChain, pMode );
}

HRESULT XDevice::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
    return m_pDevice->GetCreationParameters(pParameters);
}

HRESULT XDevice::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
    return m_pDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

//XMETHOD_(void, SetCursorPosition(int X, int Y, DWORD Flags) PURE;
//XMETHOD_(BOOL, ShowCursor)(THIS_ BOOL bShow) PURE;
//XMETHOD(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain) PURE;
//XMETHOD(GetSwapChain)(THIS_ UINT iSwapChain, IDirect3DSwapChain9** pSwapChain) PURE;
//XMETHOD_(UINT, GetNumberOfSwapChains)(THIS) PURE;
//XMETHOD(Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters) PURE;
//XMETHOD(Present)(THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) PURE;
//XMETHOD(GetBackBuffer)(THIS_ UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) PURE;
//XMETHOD(GetRasterStatus)(THIS_ UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) PURE;
//XMETHOD(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs) PURE;
//XMETHOD_(void, SetGammaRamp)(THIS_ UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp) PURE;
//XMETHOD_(void, GetGammaRamp)(THIS_ UINT iSwapChain, D3DGAMMARAMP* pRamp) PURE;
//XMETHOD(CreateTexture)(THIS_ UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) PURE;
//XMETHOD(CreateVolumeTexture)(THIS_ UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle) PURE;
//XMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) PURE;
//XMETHOD(CreateVertexBuffer)(THIS_ UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) PURE;
//XMETHOD(CreateIndexBuffer)(THIS_ UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) PURE;
//XMETHOD(CreateRenderTarget)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) PURE;
//XMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) PURE;
//XMETHOD(UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint) PURE;
//XMETHOD(UpdateTexture)(THIS_ IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) PURE;
//XMETHOD(GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) PURE;
//XMETHOD(GetFrontBufferData)(THIS_ UINT iSwapChain, IDirect3DSurface9* pDestSurface) PURE;
//XMETHOD(StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter) PURE;
//XMETHOD(ColorFill)(THIS_ IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color) PURE;
//XMETHOD(CreateOffscreenPlainSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) PURE;
HRESULT XDevice::SetRenderTarget( DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget )
{
    return m_pDevice->SetRenderTarget( RenderTargetIndex, pRenderTarget );
}
//XMETHOD(GetRenderTarget)(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) PURE;
//XMETHOD(SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil) PURE;
//XMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppZStencilSurface) PURE;
//XMETHOD(BeginScene)(THIS) PURE;
//XMETHOD(EndScene)(THIS) PURE;
//XMETHOD(Clear)(THIS_ DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) PURE;
//XMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) PURE;
//XMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) PURE;
//XMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE, CONST D3DMATRIX*) PURE;

HRESULT XDevice::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
    return m_pDevice->SetViewport(pViewport);
}

//XMETHOD(GetViewport)(THIS_ D3DVIEWPORT9* pViewport) PURE;
//XMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial) PURE;
//XMETHOD(GetMaterial)(THIS_ D3DMATERIAL9* pMaterial) PURE;
//XMETHOD(SetLight)(THIS_ DWORD Index, CONST D3DLIGHT9*) PURE;
//XMETHOD(GetLight)(THIS_ DWORD Index, D3DLIGHT9*) PURE;
//XMETHOD(LightEnable)(THIS_ DWORD Index, BOOL Enable) PURE;
//XMETHOD(GetLightEnable)(THIS_ DWORD Index, BOOL* pEnable) PURE;
//XMETHOD(SetClipPlane)(THIS_ DWORD Index, CONST float* pPlane) PURE;
//XMETHOD(GetClipPlane)(THIS_ DWORD Index, float* pPlane) PURE;
HRESULT XDevice::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
    return m_pDevice->SetRenderState(State, Value);
}
//XMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD* pValue) PURE;
//XMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB) PURE;
//XMETHOD(BeginStateBlock)(THIS) PURE;
//XMETHOD(EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB) PURE;
//XMETHOD(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus) PURE;
//XMETHOD(GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus) PURE;
//XMETHOD(GetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture9** ppTexture) PURE;

HRESULT XDevice::SetTexture( DWORD Stage, XTexture* pTexture )
{
    if(pTexture && pTexture->mTexture)
        return m_pDevice->SetTexture( Stage, pTexture->mTexture );
    return E_INVALIDARG;
}

HRESULT XDevice::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
    return m_pDevice->SetTexture( Stage, pTexture );
}

//XMETHOD(GetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) PURE;
//XMETHOD(SetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) PURE;
//XMETHOD(GetSamplerState)(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) PURE;
//XMETHOD(SetSamplerState)(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) PURE;
//XMETHOD(ValidateDevice)(THIS_ DWORD* pNumPasses) PURE;
//XMETHOD(SetPaletteEntries)(THIS_ UINT PaletteNumber, CONST PALETTEENTRY* pEntries) PURE;
//XMETHOD(GetPaletteEntries)(THIS_ UINT PaletteNumber, PALETTEENTRY* pEntries) PURE;
//XMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber) PURE;
//XMETHOD(GetCurrentTexturePalette)(THIS_ UINT* PaletteNumber) PURE;
//XMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect) PURE;
//XMETHOD(GetScissorRect)(THIS_ RECT* pRect) PURE;
//XMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware) PURE;
//XMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS) PURE;
//XMETHOD(SetNPatchMode)(THIS_ float nSegments) PURE;
//XMETHOD_(float, GetNPatchMode)(THIS) PURE;
//XMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) PURE;
//XMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) PURE;
//XMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) PURE;
//XMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) PURE;
//XMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags) PURE;
//XMETHOD(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) PURE;
////XMETHOD(SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl) PURE;
//XMETHOD(GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl) PURE;

HRESULT XDevice::SetFVF(DWORD FVF)
{
    return m_pDevice->SetFVF(FVF);
}
HRESULT XDevice::GetFVF(DWORD* pFVF)
{
    return m_pDevice->GetFVF( pFVF );
}

HRESULT XDevice::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
    return m_pDevice->CreateVertexShader( pFunction, ppShader );
}

HRESULT XDevice::SetVertexShader(IDirect3DVertexShader9* pShader)
{
    return m_pDevice->SetVertexShader( pShader );
}

HRESULT XDevice::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
    return m_pDevice->GetVertexShader( ppShader );
}

//XMETHOD(SetVertexShaderConstantF)(THIS_ UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) PURE;
//XMETHOD(GetVertexShaderConstantF)(THIS_ UINT StartRegister, float* pConstantData, UINT Vector4fCount) PURE;
//XMETHOD(SetVertexShaderConstantI)(THIS_ UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) PURE;
//XMETHOD(GetVertexShaderConstantI)(THIS_ UINT StartRegister, int* pConstantData, UINT Vector4iCount) PURE;
//XMETHOD(SetVertexShaderConstantB)(THIS_ UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount) PURE;
//XMETHOD(GetVertexShaderConstantB)(THIS_ UINT StartRegister, BOOL* pConstantData, UINT BoolCount) PURE;
//XMETHOD(SetStreamSource)(THIS_ UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) PURE;
//XMETHOD(GetStreamSource)(THIS_ UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride) PURE;
//XMETHOD(SetStreamSourceFreq)(THIS_ UINT StreamNumber, UINT Setting) PURE;
//XMETHOD(GetStreamSourceFreq)(THIS_ UINT StreamNumber, UINT* pSetting) PURE;
//XMETHOD(SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData) PURE;
//XMETHOD(GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData) PURE;

HRESULT XDevice::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
    return m_pDevice->CreatePixelShader( pFunction, ppShader );
}

HRESULT XDevice::SetPixelShader(IDirect3DPixelShader9* pShader)
{
    return m_pDevice->SetPixelShader( pShader );
}

HRESULT XDevice::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
    return m_pDevice->GetPixelShader( ppShader );
}

// 
//XMETHOD(SetPixelShaderConstantF)(THIS_ UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) PURE;
//XMETHOD(GetPixelShaderConstantF)(THIS_ UINT StartRegister, float* pConstantData, UINT Vector4fCount) PURE;
//XMETHOD(SetPixelShaderConstantI)(THIS_ UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) PURE;
//XMETHOD(GetPixelShaderConstantI)(THIS_ UINT StartRegister, int* pConstantData, UINT Vector4iCount) PURE;
//XMETHOD(SetPixelShaderConstantB)(THIS_ UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount) PURE;
//XMETHOD(GetPixelShaderConstantB)(THIS_ UINT StartRegister, BOOL* pConstantData, UINT BoolCount) PURE;
//XMETHOD(DrawRectPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo) PURE;
//XMETHOD(DrawTriPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo) PURE;
//XMETHOD(DeletePatch)(THIS_ UINT Handle) PURE;
//XMETHOD(CreateQuery)(THIS_ D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery) PURE;
