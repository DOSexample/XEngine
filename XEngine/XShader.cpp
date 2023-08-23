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
	if (s_useShaderName != name)
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
		for (auto it = this->vs->params.begin(); it != this->vs->params.end(); ++it)
			it->second = this->vs->constantTable->GetConstantByName(0, it->first.c_str());
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

void XShader::SetTexture(const char* name, XTexture* pTexture)
{
	auto device = &XDevice::Instance();
	if ( (int)this->ps->params.size() > 0 ) {
		auto px = this->ps->params[name];
		if (px && px->handle) {
			//printf("SetTexture: %s, id: %d\r\n", name, px->desc.RegisterIndex);
			device->SetTexture( px->desc.RegisterIndex, pTexture );
		}
	}
}

void XShader::SetFloatArray(const char* name, const FLOAT* pSrcData, UINT count)
{
	auto device = &XDevice::Instance();
	//printf("SetFloatArray: %s, >> size: %d\r\n", name, (int)vsParams.size());
	if ( (int)this->vs->params.size() > 0 ) {
		auto mtx = this->vs->params[name];
		if (mtx) {
			//printf("SetFloatArray: %s, >> %p\r\n", name, mtx);
			this->vs->constantTable->SetFloatArray( (LPDIRECT3DDEVICE9)device->GetRealDevice(), mtx, pSrcData, count);
		}
	}
}

void XShader::SetMatrix(const char* name, const D3DXMATRIX* pMatrix)
{
	auto device = &XDevice::Instance();
	//printf("SetMatrix: %s, >> size: %d\r\n", name, (int)vsParams.size());
	if ( (int)this->vs->params.size() > 0 ) {
		auto mtx = this->vs->params[name];
		if (mtx) {
			//printf("SetMatrix: %s, >> %p\r\n", name, mtx);
			this->vs->constantTable->SetMatrix( (LPDIRECT3DDEVICE9)device->GetRealDevice(), mtx, pMatrix );
		}
	}
}
