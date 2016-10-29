#pragma once
#include <DirectXMath.h>

//Component data struct
struct BoundingBox {
	DirectX::XMFLOAT3 m_bb[8];
};

struct MaxMin {
	DirectX::XMFLOAT3 m_max;
	DirectX::XMFLOAT3 m_min;
};