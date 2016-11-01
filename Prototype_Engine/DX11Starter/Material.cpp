#include "Material.h"

Material::Material()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	shaderResView = nullptr;
	sampler = nullptr;
}

Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* shaderResView, ID3D11SamplerState* sampler)
	: vertexShader(vertexShader), pixelShader(pixelShader), shaderResView(shaderResView), sampler(sampler)//, shaderLists(nullptr)
{
	shaderResView->AddRef();
	sampler->AddRef();
}

Material::~Material()
{
	shaderResView->Release();
	sampler->Release();
}

SimpleVertexShader* Material::getVShader()
{
	return vertexShader;
}

SimplePixelShader* Material::getPShader()
{
	return pixelShader;
}

ID3D11ShaderResourceView* Material::getShaderResView()
{
	shaderResView->AddRef();
	return shaderResView;
}

ID3D11SamplerState* Material::getSampler()
{
	sampler->AddRef();
	return sampler;
}

void Material::ReleaseShaderResView()
{
	shaderResView->Release();
}

void Material::ReleaseSampler()
{
	sampler->Release();
}