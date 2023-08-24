#pragma once

#include "XDevice.h"

#include <map>
#include <string>

enum class ShaderType
{
	Vertex,
	Pixel,
};
enum class ShaderSetType
{
	SetTexture,
	SetInt,
	SetIntArray,
	SetFloat,
	SetFloatArray,
	SetMatrix,
	SetMatrixArray,
};

class XShader
{

public:

	XShader();
	~XShader();
	void Release();

	HRESULT CreateFromFile(const char* name, const char* vsFile, const char* psFile);
	HRESULT Create(ShaderType type, const char* fileName);

	void AddVertexInput(const char* name);
	void AddPixelInput(const char* name);

	void Set(ShaderType shadertype, ShaderSetType setType, const char* name, LPCVOID pSrcData, UINT count = 0);

	int id;
	std::string name;

	struct VS{
		std::string fileName = "";
		ID3DXConstantTable* constantTable = nullptr;
		IDirect3DVertexShader9* shader = nullptr;
		std::map<std::string, D3DXHANDLE> params = std::map<std::string, D3DXHANDLE>();
	};
	VS *vs;

	struct PixelDesc {
		D3DXHANDLE handle = D3DXHANDLE();
		D3DXCONSTANT_DESC desc = D3DXCONSTANT_DESC();
	};
	struct PS{
		std::string fileName = "";
		ID3DXConstantTable* constantTable = nullptr;
		IDirect3DPixelShader9* shader = nullptr;
		std::map<std::string, PixelDesc*> params = std::map<std::string, PixelDesc*>();
	};
	PS *ps;

	static bool IsValidVertex(XShader* xs) { return xs && xs->vs && xs->vs->shader && xs->vs->constantTable; }
	static bool IsValidPixel(XShader* xs) { return xs && xs->ps && xs->ps->shader && xs->ps->constantTable; }
};

class XShaderManager
{
public:


	static void Add(XShader* shader);
	static XShader* Get(const std::string& name);
	static void SetShader(const std::string& name, XShader* xs);
};