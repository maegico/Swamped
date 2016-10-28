#pragma once
#include "System.h"
#include "CollisionComponent.h"
#include <DirectXMath.h>

//A System implementation
class CollisionSystem : public System<CollisionComponent> {
public:
	void Update(Game * g, float dT);
	CollisionSystem();
	~CollisionSystem();
private:
	vector<CollisionComponent> m_aabbs;
};