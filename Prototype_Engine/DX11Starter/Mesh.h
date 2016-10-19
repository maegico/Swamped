#pragma once
#include <cstdint>
#include <d3d11.h>

#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	Mesh(Vertex* vertArray, uint32_t* indexArray, uint32_t sizeVertArray, uint32_t sizeIndexArray, ID3D11Device* device);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	uint32_t GetIndexCount();
private:
	//pointers to the Vertex Buffer and Index Buffer
	ID3D11Buffer* vertBuf;
	ID3D11Buffer* indexBuf;
	int numIndices;	//used when drawing
};

