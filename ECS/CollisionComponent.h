#pragma once
#include <DirectXMath.h>

//Points in a bounding box
struct BoundingBox {
	DirectX::XMFLOAT3 m_bb[8];
};

//Represents the max and min of an AABB
struct MaxMin {
	DirectX::XMFLOAT3 m_max;
	DirectX::XMFLOAT3 m_min;
};