#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace std;
typedef uint8_t CollisionMask;
enum CollisionType { none, immobile };

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