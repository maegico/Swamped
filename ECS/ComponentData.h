#pragma once
#include "EntityIdTypeDef.h"

//Wrapper for component data
class ComponentData {
public:
	//Whether the component is active
	bool m_active = true;
	ComponentData() {}
	ComponentData(EntityId entityId) {
		m_entityId = entityId;
	}

	//Getter for entity ID
	EntityId GetEntityId() {
		return m_entityId;
	}
private:
	EntityId m_entityId;
};
