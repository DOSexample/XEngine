#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if( x ){ x->Release(); x = NULL; }
#endif

enum ClearType : DWORD
{
	NONE,
	TARGET = D3DCLEAR_TARGET,
	ZBUFFER = D3DCLEAR_ZBUFFER,
	STENCIL = D3DCLEAR_STENCIL,
	ALL = TARGET | ZBUFFER | STENCIL,
};

enum ColorType : DWORD
{
    White = D3DCOLOR_RGBA(0, 0, 0, 0),
	Black = D3DCOLOR_RGBA(0, 0, 0, 1),
	Grey = D3DCOLOR_RGBA(128, 128, 128, 1),
	Pink = D3DCOLOR_RGBA(245, 40, 145, 1),
};

class XCamera;
class XTexture;

class XDevice
{
public:
	static XDevice& Instance() {
		static XDevice x;
		return x;
	}

	XDevice();
	~XDevice();


    void* GetRealDevice();
	bool Initialize(void* hWnd, int width, int height);
	void Clear(ClearType clearType = ALL, ColorType colorType = Pink, float z = 1.0f, DWORD Stencil = 0);

    D3DXMATRIX GetWorldMatrix();
    D3DXMATRIX GetViewMatrix();
    D3DXMATRIX GetPerspectiveMatrix();
    void SetCamera(XCamera* cam);
    const D3DLIGHT9 GetDefaultLight();

	BOOL CreateVertexDeclaration();
    BOOL SetVertexDeclaration();

	BOOL CreateTexture(	
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
	);


    /*** IDirect3DDevice9 methods ***/
    #define XMETHOD(x) void x
    #define XMETHOD_(t, x) t x
    #undef PURE
    #define PURE
    HRESULT TestCooperativeLevel();
    UINT GetAvailableTextureMem();
    HRESULT EvictManagedResources();
    HRESULT GetDirect3D(IDirect3D9** ppD3D9);
    HRESULT GetDeviceCaps(D3DCAPS9* pCaps);
    HRESULT GetDisplayMode( UINT iSwapChain, D3DDISPLAYMODE* pMode);
    HRESULT GetCreationParameters( D3DDEVICE_CREATION_PARAMETERS* pParameters);
    HRESULT SetCursorProperties( UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap);
    XMETHOD_(void, SetCursorPosition)(THIS_ int X, int Y, DWORD Flags) PURE;
    XMETHOD_(BOOL, ShowCursor)(THIS_ BOOL bShow) PURE;
    XMETHOD(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain) PURE;
    XMETHOD(GetSwapChain)(THIS_ UINT iSwapChain, IDirect3DSwapChain9** pSwapChain) PURE;
    XMETHOD_(UINT, GetNumberOfSwapChains)(THIS) PURE;
    XMETHOD(Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters) PURE;
    XMETHOD(Present)(THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) PURE;
    XMETHOD(GetBackBuffer)(THIS_ UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) PURE;
    XMETHOD(GetRasterStatus)(THIS_ UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) PURE;
    XMETHOD(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs) PURE;
    XMETHOD_(void, SetGammaRamp)(THIS_ UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp) PURE;
    XMETHOD_(void, GetGammaRamp)(THIS_ UINT iSwapChain, D3DGAMMARAMP* pRamp) PURE;
    XMETHOD(CreateTexture)(THIS_ UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) PURE;
    XMETHOD(CreateVolumeTexture)(THIS_ UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle) PURE;
    XMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) PURE;
    XMETHOD(CreateVertexBuffer)(THIS_ UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) PURE;
    XMETHOD(CreateIndexBuffer)(THIS_ UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) PURE;
    XMETHOD(CreateRenderTarget)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) PURE;
    XMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) PURE;
    XMETHOD(UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint) PURE;
    XMETHOD(UpdateTexture)(THIS_ IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) PURE;
    XMETHOD(GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) PURE;
    XMETHOD(GetFrontBufferData)(THIS_ UINT iSwapChain, IDirect3DSurface9* pDestSurface) PURE;
    XMETHOD(StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter) PURE;
    XMETHOD(ColorFill)(THIS_ IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color) PURE;
    XMETHOD(CreateOffscreenPlainSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) PURE;
    HRESULT SetRenderTarget( DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget );
    XMETHOD(GetRenderTarget)(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) PURE;
    XMETHOD(SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil) PURE;
    XMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppZStencilSurface) PURE;
    XMETHOD(BeginScene)(THIS) PURE;
    XMETHOD(EndScene)(THIS) PURE;
    XMETHOD(Clear)(THIS_ DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) PURE;
    XMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) PURE;
    XMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) PURE;
    XMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE, CONST D3DMATRIX*) PURE;
    HRESULT SetViewport(CONST D3DVIEWPORT9* pViewport);
    XMETHOD(GetViewport)(THIS_ D3DVIEWPORT9* pViewport) PURE;
    XMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial) PURE;
    XMETHOD(GetMaterial)(THIS_ D3DMATERIAL9* pMaterial) PURE;
    XMETHOD(SetLight)(THIS_ DWORD Index, CONST D3DLIGHT9*) PURE;
    XMETHOD(GetLight)(THIS_ DWORD Index, D3DLIGHT9*) PURE;
    XMETHOD(LightEnable)(THIS_ DWORD Index, BOOL Enable) PURE;
    XMETHOD(GetLightEnable)(THIS_ DWORD Index, BOOL* pEnable) PURE;
    XMETHOD(SetClipPlane)(THIS_ DWORD Index, CONST float* pPlane) PURE;
    XMETHOD(GetClipPlane)(THIS_ DWORD Index, float* pPlane) PURE;
    HRESULT SetRenderState( D3DRENDERSTATETYPE State, DWORD Value );
    XMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD* pValue) PURE;
    XMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB) PURE;
    XMETHOD(BeginStateBlock)(THIS) PURE;
    XMETHOD(EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB) PURE;
    XMETHOD(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus) PURE;
    XMETHOD(GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus) PURE;
    XMETHOD(GetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture9** ppTexture) PURE;
    HRESULT SetTexture( DWORD Stage, XTexture* pTexture);
    HRESULT SetTexture( DWORD Stage, IDirect3DBaseTexture9* pTexture);
    XMETHOD(GetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) PURE;
    XMETHOD(SetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) PURE;
    XMETHOD(GetSamplerState)(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) PURE;
    XMETHOD(SetSamplerState)(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) PURE;
    XMETHOD(ValidateDevice)(THIS_ DWORD* pNumPasses) PURE;
    XMETHOD(SetPaletteEntries)(THIS_ UINT PaletteNumber, CONST PALETTEENTRY* pEntries) PURE;
    XMETHOD(GetPaletteEntries)(THIS_ UINT PaletteNumber, PALETTEENTRY* pEntries) PURE;
    XMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber) PURE;
    XMETHOD(GetCurrentTexturePalette)(THIS_ UINT* PaletteNumber) PURE;
    XMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect) PURE;
    XMETHOD(GetScissorRect)(THIS_ RECT* pRect) PURE;
    XMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware) PURE;
    XMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS) PURE;
    XMETHOD(SetNPatchMode)(THIS_ float nSegments) PURE;
    XMETHOD_(float, GetNPatchMode)(THIS) PURE;
    XMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) PURE;
    XMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) PURE;
    XMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) PURE;
    XMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) PURE;
    XMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags) PURE;
    XMETHOD(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) PURE;
    //XMETHOD(SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl) PURE;
    XMETHOD(GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl) PURE;
    HRESULT SetFVF( DWORD FVF);
    HRESULT GetFVF( DWORD* pFVF);
    HRESULT CreateVertexShader( CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader );
    HRESULT SetVertexShader( IDirect3DVertexShader9* pShader );
    HRESULT GetVertexShader( IDirect3DVertexShader9** ppShader );
    XMETHOD(SetVertexShaderConstantF)(THIS_ UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) PURE;
    XMETHOD(GetVertexShaderConstantF)(THIS_ UINT StartRegister, float* pConstantData, UINT Vector4fCount) PURE;
    XMETHOD(SetVertexShaderConstantI)(THIS_ UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) PURE;
    XMETHOD(GetVertexShaderConstantI)(THIS_ UINT StartRegister, int* pConstantData, UINT Vector4iCount) PURE;
    XMETHOD(SetVertexShaderConstantB)(THIS_ UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount) PURE;
    XMETHOD(GetVertexShaderConstantB)(THIS_ UINT StartRegister, BOOL* pConstantData, UINT BoolCount) PURE;
    XMETHOD(SetStreamSource)(THIS_ UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) PURE;
    XMETHOD(GetStreamSource)(THIS_ UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride) PURE;
    XMETHOD(SetStreamSourceFreq)(THIS_ UINT StreamNumber, UINT Setting) PURE;
    XMETHOD(GetStreamSourceFreq)(THIS_ UINT StreamNumber, UINT* pSetting) PURE;
    XMETHOD(SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData) PURE;
    XMETHOD(GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData) PURE;
    HRESULT CreatePixelShader( CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader);
    HRESULT SetPixelShader( IDirect3DPixelShader9* pShader );
    HRESULT GetPixelShader( IDirect3DPixelShader9** ppShader );
    XMETHOD(SetPixelShaderConstantF)(THIS_ UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) PURE;
    XMETHOD(GetPixelShaderConstantF)(THIS_ UINT StartRegister, float* pConstantData, UINT Vector4fCount) PURE;
    XMETHOD(SetPixelShaderConstantI)(THIS_ UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) PURE;
    XMETHOD(GetPixelShaderConstantI)(THIS_ UINT StartRegister, int* pConstantData, UINT Vector4iCount) PURE;
    XMETHOD(SetPixelShaderConstantB)(THIS_ UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount) PURE;
    XMETHOD(GetPixelShaderConstantB)(THIS_ UINT StartRegister, BOOL* pConstantData, UINT BoolCount) PURE;
    XMETHOD(DrawRectPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo) PURE;
    XMETHOD(DrawTriPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo) PURE;
    XMETHOD(DeletePatch)(THIS_ UINT Handle) PURE;
    XMETHOD(CreateQuery)(THIS_ D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery) PURE;

    #undef XMETHOD
    #undef XMETHOD_

    #undef PURE

};

