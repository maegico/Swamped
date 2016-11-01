#pragma once
#include <DirectXMath.h>
#include <cstdint>
#include "Material.h"
#include "Mesh.h"
#include "SimpleShader.h"
#include "Lights.h"

//Will change to use Renderer to render
//Work on making the Entity classes as more of a dumb container and don't let it draw itself

//will keep track of its own material

class Entity
{
public:
	Entity();
	Entity(ID3D11DeviceContext* context, Mesh* mesh, Material* mat, DirectX::XMFLOAT3 position, uint32_t arrayPosition);
	~Entity();
	
	DirectX::XMFLOAT4X4 getWorldMatrix();
	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT3 getRotation();
	DirectX::XMFLOAT3 getScale();
	Material* getMat();
	Mesh* getMesh() { return mesh; };

	void setWorldMatrix(DirectX::XMFLOAT4X4 worldMat);
	void setPosition(DirectX::XMFLOAT3 position);
	void setRotation(DirectX::XMFLOAT3 rotation);
	void setScale(DirectX::XMFLOAT3 scale);
	void move(DirectX::XMFLOAT3 offset);


	void update(float deltaTime);
	void draw();

private:
	ID3D11DeviceContext* context;
	Mesh* mesh;
	Material* mat;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT3 position, rotation, scale;
	DirectionalLight dirLight;
	uint32_t arrayPosition;
	
};

