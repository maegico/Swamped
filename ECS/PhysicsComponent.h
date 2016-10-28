#pragma once

#include <DirectXMath.h>

struct PhysicsComponent {
	DirectX::XMFLOAT3 m_velocity = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 m_acceleration = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT4 m_rotationalVelocity = DirectX::XMFLOAT4(0, 0, 0, 1);
	DirectX::XMFLOAT4 m_rotationalAcceleration = DirectX::XMFLOAT4(0, 0, 0, 1);
};
