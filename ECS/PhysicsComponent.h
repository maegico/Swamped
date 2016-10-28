#pragma once

#include <DirectXMath.h>

struct PhysicsComponent {
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_acceleration;
};
