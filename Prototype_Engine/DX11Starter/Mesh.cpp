#include "Mesh.h"
#include <fstream>
#include <vector>
#include <DirectXMath.h>

//Going with the dumb container method
//will hold 

Mesh::Mesh() : vertBuf(nullptr), indexBuf(nullptr), numIndices(0)
{ }

Mesh::Mesh(Vertex* vertArray, uint32_t* indexArray, uint32_t sizeVertArray, uint32_t sizeIndexArray, ID3D11Device* device) : numIndices(sizeIndexArray)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * sizeVertArray;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertArray;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&vbd, &initialVertexData, &vertBuf);


	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(uint32_t) * sizeIndexArray;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indexArray;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&ibd, &initialIndexData, &indexBuf);
}

Mesh::~Mesh()
{
	//due to me creating them, it creates the reference automatically and then I'm releasing it
	vertBuf->Release();
	indexBuf->Release();
}

ID3D11Buffer* Mesh::GetVertexBuffer()
{
	vertBuf->AddRef();
	return vertBuf;
}

ID3D11Buffer* Mesh::GetIndexBuffer()
{
	indexBuf->AddRef();
	return indexBuf;
}

uint32_t Mesh::GetIndexCount()
{
	return numIndices;
}
