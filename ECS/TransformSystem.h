#pragma once

#include "PairedSystem.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include <DirectXMath.h>
using namespace DirectX;
class TransformSystem : public PairedSystem<TransformComponent, PhysicsComponent> {
public:
	void Update(Game * g, float dT);
	static DirectX::XMMATRIX GetMatrix(TransformComponent tc);
	size_t GetSize();
	size_t GetCount();
};
