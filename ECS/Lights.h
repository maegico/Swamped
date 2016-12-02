#pragma once
#include <DirectXMath.h>

struct DirectionalLight
{
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Direction;
	float nope;
};

struct PointLight
{
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Position;
	float DiffuseIntensity;
};

struct SpotLight
{
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Position;
	float DiffuseIntensity;
	DirectX::XMFLOAT3 Direction;
	float nope;
};

struct Lights
{
	SpotLight spotLights[8];
	PointLight pointLights[8];
	DirectionalLight dirLight;
	DirectX::XMFLOAT4 AmbientColor;
	//look into ints on the GPU and if they are okay
	int sizePointLights;
	int sizeSpotLights;
};
