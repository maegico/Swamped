#include "Entity.h"

Entity::Entity()
{
	mesh = nullptr;
	context = nullptr;

	position = DirectX::XMFLOAT3(0, 0, 0);
	scale = DirectX::XMFLOAT3(1, 1, 1);
	rotation = DirectX::XMFLOAT3(0, 0, 1);
}

Entity::Entity(ID3D11DeviceContext* context, Mesh* mesh, Material* mat, DirectX::XMFLOAT3 position, uint32_t arrayPosition)
	: context(context), mesh(mesh), mat(mat), position(position), arrayPosition(arrayPosition)
{
	context->AddRef();
	scale = DirectX::XMFLOAT3(0.25, 0.25, 0.25);
	rotation = DirectX::XMFLOAT3(0, 0, 1);
}

Entity::~Entity()
{
	context->Release();
}

DirectX::XMFLOAT4X4 Entity::getWorldMatrix()
{
	return worldMatrix;
}

DirectX::XMFLOAT3 Entity::getPosition()
{
	return position;
}

DirectX::XMFLOAT3 Entity::getRotation()
{
	return rotation;
}

DirectX::XMFLOAT3 Entity::getScale()
{
	return scale;
}

Material* Entity::getMat()
{
	return mat;
}

void Entity::setWorldMatrix(DirectX::XMFLOAT4X4 worldMat)
{
	this->worldMatrix = worldMat;
}

void Entity::setPosition(DirectX::XMFLOAT3 position)
{
	this->position = position;
}

void Entity::setRotation(DirectX::XMFLOAT3 rotation)
{
	this->rotation = rotation;
}

void Entity::setScale(DirectX::XMFLOAT3 scale)
{
	this->scale = scale;
}

void Entity::move(DirectX::XMFLOAT3 offset)
{
	position.x += offset.x;
	position.y += offset.y;
	position.z += offset.z;
}

void Entity::update(float deltaTime)
{
	DirectX::XMVECTOR tempPosition = XMLoadFloat3(&position);
	DirectX::XMVECTOR tempRotation = XMLoadFloat3(&rotation);
	DirectX::XMVECTOR tempScale = XMLoadFloat3(&scale);


	DirectX::XMMATRIX tempWorldMatrix;/*
	DirectX::XMMATRIX translationMatrix;
	DirectX::XMMATRIX scalingMatrix;
	DirectX::XMMATRIX rotationMatrix;*/

	tempWorldMatrix = DirectX::XMMatrixAffineTransformation(tempScale, DirectX::XMVectorZero(), tempRotation, tempPosition);
/*
	translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	scalingMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	rotationMatrix = DirectX::XMMatrixRotationAxis(tempRotation, 0.0f);

	tempWorldMatrix = DirectX::XMMatrixMultiply(translationMatrix, scalingMatrix);
	tempWorldMatrix = DirectX::XMMatrixMultiply(tempWorldMatrix, rotationMatrix);*/

	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixTranspose(tempWorldMatrix));
}

void Entity::draw()
{
	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	mat->getVShader()->SetMatrix4x4("world", worldMatrix);	//needs to be created every frame and uploaded every frame


	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	mat->getVShader()->CopyAllBufferData();
	mat->getPShader()->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	mat->getVShader()->SetShader();
	mat->getPShader()->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ID3D11Buffer* vertbuf = mesh->GetVertexBuffer();
	ID3D11Buffer* indexbuf = mesh->GetIndexBuffer();

	context->IASetVertexBuffers(0, 1, &vertbuf, &stride, &offset);
	context->IASetIndexBuffer(indexbuf, DXGI_FORMAT_R32_UINT, 0);

	/*Finally do the actual drawing
	- Do this ONCE PER OBJECT you intend to draw
	- This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	- DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	vertices in the currently set VERTEX BUFFER*/
	context->DrawIndexed(
		mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

	vertbuf->Release();
	indexbuf->Release();
}
