#pragma once

#include <DirectXMath.h>

struct Particle {
	DirectX::XMFLOAT3 m_startPosition = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 m_velocity = DirectX::XMFLOAT3(0, 0, 0);
	float m_size = 1;
	float m_birthTime = 0;
	//DirectX::XMFLOAT3 m_color;
};
