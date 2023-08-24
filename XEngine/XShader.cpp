#include "XShader.h"

#include "XDevice.h"

#include "XLoader/FileLoader.h"

std::map<std::string, XShader*> s_Shader;
int s_ShaderID = 0;
std::string s_useShaderName = "";
XShader* s_lastShader = nullptr;

void XShaderManager::Add(XShader* shader)
{
	s_Shader.insert( { shader->name, shader } );
	shader->id = s_ShaderID++;
}

XShader* XShaderManager::Get(const std::string& name)
{
	if (s_useShaderName == name)
		return s_lastShader;

	auto it = s_Shader.find(name);
	auto xs = it != s_Shader.end() ? it->second : nullptr;
	if (xs && s_useShaderName != name)
	{
		s_lastShader = xs;
		s_useShaderName = name;

		auto device = &XDevice::Instance();

		if (XShader::IsValidVertex(xs))
			device->SetVertexShader(xs->vs->shader);

		if (XShader::IsValidPixel(xs))
			device->SetPixelShader(xs->ps->shader);
	}
	return xs;
}

XShader::XShader()
	:
	id(0),
	name(""),
	vs(new VS),
	ps(new PS)
{

}

XShader::~XShader()
{
	Release();
}

void XShader::Release()
{
	if (vs) {
		vs->params.clear();
		SAFE_RELEASE( vs->constantTable );
		SAFE_RELEASE( vs->shader );
		delete vs;
		vs = nullptr;
	}

	if (ps) {
		for (auto it = ps->params.begin(); it != ps->params.end(); ++it)
			delete it->second;
		ps->params.clear();
		SAFE_RELEASE( ps->constantTable );
		SAFE_RELEASE( ps->shader );
		delete ps;
		ps = nullptr;
	}
}

HRESULT XShader::CreateFromFile(const char* name, const char* vsFile, const char* psFile)
{
	if (strlen(vsFile)) {
		if (FAILED(Create(ShaderType::Vertex, vsFile)))
		{
			Release();
			return E_ABORT;
		}
	}
	if (strlen(psFile)) {
		if (FAILED(Create(ShaderType::Pixel, psFile)))
		{
			Release();
			return E_ABORT;
		}
	}

	this->name = name;
	XShaderManager::Add(this);

	return NOERROR;
}

HRESULT XShader::Create(ShaderType type, const char* fileName)
{
	auto device = &XDevice::Instance();
	ID3DXBuffer* ppShader = nullptr;
	ID3DXBuffer* ppErrorMsgs = nullptr;
	DWORD compileFlag = D3DXSHADER_SKIPOPTIMIZATION | D3DXSHADER_USE_LEGACY_D3DX9_31_DLL;
	std::string st = type == ShaderType::Vertex ? "vs_2_0" : "ps_2_0";
	ID3DXConstantTable** constanTable = type == ShaderType::Vertex ? &this->vs->constantTable : &this->ps->constantTable;

	D3DXCompileShaderFromFileA(fileName, 0, 0, "Main", st.c_str(), compileFlag, &ppShader, &ppErrorMsgs, constanTable);
	if (ppErrorMsgs)
	{
		printf("XShader::CreateFromFile() -> %s \r\n", (CONST CHAR*)ppErrorMsgs->GetBufferPointer());
		ppErrorMsgs->Release();
		ppErrorMsgs = nullptr;
		return E_ABORT;
	}
	if (constanTable == nullptr)
		return E_NOTIMPL;

	HRESULT hr = NOERROR;
	if (type == ShaderType::Vertex)
		hr = device->CreateVertexShader((const DWORD*)ppShader->GetBufferPointer(), &this->vs->shader);
	else
		hr = device->CreatePixelShader((const DWORD*)ppShader->GetBufferPointer(), &this->ps->shader);

	if (FAILED(hr))
		return hr;

	printf("%p, %p \r\n", this->vs->constantTable, this->ps->constantTable);
	if (type == ShaderType::Vertex)
	{
		for (auto it = this->vs->params.begin(); it != this->vs->params.end(); ++it) {
			it->second = this->vs->constantTable->GetConstantByName(0, it->first.c_str());
		}
	}
	else
	{
		for (auto it = this->ps->params.begin(); it != this->ps->params.end(); ++it)
		{
			it->second->handle = this->ps->constantTable->GetConstantByName(0, it->first.c_str());
			if (it->second->handle)
			{
				UINT count;
				this->ps->constantTable->GetConstantDesc(it->second->handle, &it->second->desc, &count);
			}
		}
	}

	return hr;
}

void XShader::AddVertexInput(const char* name)
{
	auto it = this->vs->params.find(name);
	if (it == this->vs->params.end())
		this->vs->params.insert({ name, D3DXHANDLE{} });
}

void XShader::AddPixelInput(const char* name)
{
	auto it = this->ps->params.find(name);
	if (it == this->ps->params.end())
		this->ps->params.insert({ name, new PixelDesc{ D3DXHANDLE{}, D3DXCONSTANT_DESC{} } });
}

void XShader::Set(ShaderType shadertype, ShaderSetType setType, const char* name, LPCVOID pSrcData, UINT count)
{
	auto device = &XDevice::Instance();
	auto real_device = (LPDIRECT3DDEVICE9)device->GetRealDevice();
	ID3DXConstantTable* constantTable = NULL;
	D3DXHANDLE handle = NULL;
	D3DXCONSTANT_DESC* desc = NULL;

	if (shadertype == ShaderType::Vertex)
	{
		constantTable = this->vs->constantTable;

		auto it = this->vs->params.find(name);
		if (it != this->vs->params.end()) {
			handle = it->second;
		}
	}
	else
	{
		constantTable = this->ps->constantTable;

		auto it = this->ps->params.find(name);
		if (it != this->ps->params.end()) {
			handle = it->second->handle;
			desc = &it->second->desc;
		}
	}

	//printf("XShader::Set() -> %d,%d,%s,%p,%p\n", shadertype, setType, name,handle, desc);

	if (!handle) return;

	switch ( setType )
	{
	case ShaderSetType::SetTexture:
		if( desc )
			device->SetTexture( desc->RegisterIndex, (XTexture*)pSrcData );
		break;
	case ShaderSetType::SetInt:
		constantTable->SetInt( real_device, handle, *((INT*)( pSrcData )) );
		break;
	case ShaderSetType::SetIntArray:
		constantTable->SetIntArray( real_device, handle, ((const INT*)( pSrcData )), count );
		break;
	case ShaderSetType::SetFloat:
		constantTable->SetFloat( real_device, handle, *((FLOAT*)( pSrcData )) );
		break;
	case ShaderSetType::SetFloatArray:
		constantTable->SetFloatArray( real_device, handle, ((const FLOAT*)( pSrcData )), count );
		break;
	case ShaderSetType::SetMatrix:
		constantTable->SetMatrix( real_device, handle, ((const D3DXMATRIX*)pSrcData ));
		break;
	case ShaderSetType::SetMatrixArray:
		constantTable->SetMatrixArray( real_device, handle, ((const D3DXMATRIX*)( pSrcData )), count );
		break;
	}
}
