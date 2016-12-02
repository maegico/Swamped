#pragma once
#include "EntityIdTypeDef.h"
#include "ComponentData.h"
#include <DirectXMath.h>
#include <vector>
using namespace std;
typedef uint8_t CollisionMask;
enum CollisionType { none, terrain, ghost, player, NUMTYPES };

//CollisionMask MakeCollisionMask(vector<CollisionType> types);

//Points in a bounding box
struct BoundingBox {
	DirectX::XMFLOAT3 m_bb[8];
	CollisionType m_ct;
	//CollisionMask m_cm;
};

//Represents the max and min of an AABB
struct MaxMin {
	DirectX::XMFLOAT3 m_max;
	DirectX::XMFLOAT3 m_min;
	//CollisionMask m_cm;
};

struct TypedMaxMin : public MaxMin {
	CollisionType m_ct;

	operator MaxMin() const {
		return{ m_max,m_min };
	}
};