#pragma once

#include <DirectXMath.h>

struct Particle {
	DirectX::XMFLOAT3 m_position = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 m_velocity = DirectX::XMFLOAT3(0, 0, 0);
	float m_size = 1;
	//DirectX::XMFLOAT3 m_color;
};
