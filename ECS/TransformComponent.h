#pragma once

#include <DirectXMath.h>

struct TransformComponent {
	DirectX::XMFLOAT3 m_position = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT4 m_rotation = DirectX::XMFLOAT4(0, 0, 0, 1);
};
