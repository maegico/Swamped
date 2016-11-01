#pragma once
#include "SimpleShader.h"
#include "Shaders.h"

//represents a set of vertex and pixel shaders

class Material
{
public:
	// Default Constructor that does nothing
	// @return nothing
	//
	int ghat;
	Material();
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* shaderResView, ID3D11SamplerState* sampler);
	/*Material(int count, ID3D11ShaderResourceView* shaderResView, ID3D11SamplerState* sampler);*/
	//Material(ID3D11ShaderResourceView* shaderResView, ID3D11SamplerState* sampler, Shaders* shaderLists);
	~Material();

	SimpleVertexShader* getVShader();
	SimplePixelShader* getPShader();
	//SimpleComputeShader* getCShader();
	//SimpleGeometryShader* getGShader();
	//SimpleDomainShader* getDShader();
	//SimpleHullShader* getHShader();

	ID3D11ShaderResourceView* getShaderResView();
	ID3D11SamplerState* getSampler();
	void ReleaseShaderResView();
	void ReleaseSampler();

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* shaderResView;
	ID3D11SamplerState* sampler;
	//Shaders* shaderLists;
};