#pragma once
#include "System.h"
#include "CollisionComponent.h"
#include <DirectXMath.h>

//A System implementation
class CollisionSystem : public System<BoundingBox> {
public:
	//Generates AABBs and checks collision
	void Update(Game * g, float dT);
	CollisionSystem();
	~CollisionSystem();
private:
	//Pre-allocated list of the current frame's AABBs
	vector<MaxMin> m_aabbs;
};