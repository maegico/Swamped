#pragma once

#include <DirectXMath.h>

//Represents a set of physics quantities
struct PhysicsComponent {
	DirectX::XMFLOAT3 m_velocity = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 m_acceleration = DirectX::XMFLOAT3(0, 0, 0);
	//DirectX::XMFLOAT3 m_rotationalVelocity = DirectX::XMFLOAT3(0, 0, 0);
	//DirectX::XMFLOAT3 m_rotationalAcceleration = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 m_rotationalVelocity = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 m_rotationalAcceleration = DirectX::XMFLOAT3(0, 0, 0);
};
