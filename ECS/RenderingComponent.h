#pragma once
#include "SimpleShader.h"
#include "FreeVector.h"
#include "ComponentData.h"

struct Material {
	SimpleVertexShader * vertexShader;
	SimplePixelShader * pixelShader;
	ID3D11ShaderResourceView * textureView;
	ID3D11ShaderResourceView* normalMap;
	ID3D11SamplerState * samplerState;
};

struct ParticleMaterial {
	SimpleVertexShader * vertexShader;
	SimpleGeometryShader * geometryShader;
	SimplePixelShader * pixelShader;
	ID3D11ShaderResourceView * textureView;
	ID3D11SamplerState * samplerState;
};

struct SkyBoxMaterial {
	SimpleVertexShader * vertexShader;
	SimplePixelShader * pixelShader;
	ID3D11ShaderResourceView * textureView;
	ID3D11SamplerState * samplerState;
};

struct Mesh {
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	unsigned int indexCount;
};

struct RenderingComponent {
	Material m_material;
	Mesh m_mesh;
};

struct SkyBoxComponent {
	SkyBoxMaterial m_material;
	Mesh m_mesh;
};

struct RenderingHandle {
	FreeVector<ComponentData> * m_rcList;
	unsigned int m_index;
};
