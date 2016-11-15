#pragma once
#include "ComponentData.h"
#include "GameForwardDecl.h"
#include "EntityIdTypeDef.h"
#include <vector>
using namespace std;
class ISystem {
public:
	void virtual Remove(EntityId entityId) = 0;
};