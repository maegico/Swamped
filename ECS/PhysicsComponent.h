#pragma once

#include <DirectXMath.h>

struct PhysicsComponent {
	DirectX::XMFLOAT3 m_velocity = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 m_acceleration = DirectX::XMFLOAT3(1, 0, 0);
};
