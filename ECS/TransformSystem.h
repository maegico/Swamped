#pragma once

#include "PairedSystem.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "EntityIdTypeDef.h"
#include <DirectXMath.h>
#include <ppl.h>
using namespace Concurrency;
using namespace DirectX;
class TransformSystem : public PairedSystem<TransformComponent, PhysicsComponent> {
public:
	void Update(Game * g, float dT);
	static DirectX::XMMATRIX GetMatrix(TransformComponent& tc);
private:
	float m_gravity = -15;
};
