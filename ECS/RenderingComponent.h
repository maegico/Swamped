#pragma once
#include "SimpleShader.h"

struct Material {
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
