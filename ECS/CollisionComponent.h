#pragma once
#include <DirectXMath.h>
#include <vector>

typedef uint8_t CollisionMask;
enum CollisionType { none };

CollisionMask MakeCollisionMask(vector<CollisionType> types);



//Points in a bounding box
struct BoundingBox {
	DirectX::XMFLOAT3 m_bb[8];
	CollisionMask m_cm;
};

//Represents the max and min of an AABB
struct MaxMin {
	DirectX::XMFLOAT3 m_max;
	DirectX::XMFLOAT3 m_min;
	CollisionMask m_cm;
};