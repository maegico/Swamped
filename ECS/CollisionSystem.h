#pragma once
#include "System.h"
#include "CollisionComponent.h"
#include <DirectXMath.h>

//A System implementation
class CollisionSystem : public System<CollisionComponent> {
public:
	void Update(Game * g);
	CollisionSystem();
	~CollisionSystem();
private:
	vector<CollisionComponent> m_aabbs;
};