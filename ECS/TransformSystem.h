#pragma once

#include "PairedSystem.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"

class TransformSystem : public PairedSystem<TransformComponent, PhysicsComponent> {
public:
	void Update(Game * g);
	size_t GetSize();
	size_t GetCount();
};
