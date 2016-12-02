#pragma once
#include "EntityIdTypeDef.h"

//Wrapper for component data
struct ComponentData {
	//Whether the component is active
	bool m_active;
	EntityId m_entityId;
};
