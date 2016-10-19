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

//Material::Material(ID3D11ShaderResourceView* shaderResView, ID3D11SamplerState* sampler, Shaders* shaderLists)
//	: vertexShader(nullptr), pixelShader(nullptr), shaderResView(shaderResView), sampler(sampler), shaderLists(shaderLists)
//{
//	shaderResView->AddRef();
//	sampler->AddRef();
//}


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

//SimpleComputeShader * Material::getCShader()
//{
//	return shaderLists->cs;
//}
//
//SimpleGeometryShader * Material::getGShader()
//{
//	return shaderLists->gs;
//}
//
//SimpleDomainShader * Material::getDShader()
//{
//	return shaderLists->ds;
//}
//
//SimpleHullShader * Material::getHShader()
//{
//	return shaderLists->hs;
//}

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

//void Material::setShaderResView(ID3D11ShaderResourceView* shaderResView)
//{
//	this->shaderResView = shaderResView;
//}
//
//void Material::setSampler(ID3D11SamplerState* sampler)
//{
//	this->sampler = sampler;
//}
