#pragma once
#include "System.h"
#include "CollisionComponent.h"
#include <DirectXMath.h>

//A System implementation
class CollisionSystem : public System<BoundingBox> {
public:
	void Update(Game * g, float dT);
	CollisionSystem();
	~CollisionSystem();
private:
	vector<MaxMin> m_aabbs;
};