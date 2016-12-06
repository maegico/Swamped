#pragma once

#include <DirectXMath.h>

struct Particle {
	DirectX::XMFLOAT3 m_position = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 m_velocity = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT2 m_size = DirectX::XMFLOAT2(1, 1);
	//DirectX::XMFLOAT3 m_color;
};
