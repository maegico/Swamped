#pragma once

//Wrapper for component data
class ComponentData {
public:
	//Whether the component is active
	bool m_active = false;
	ComponentData() {}
	ComponentData(unsigned int entityId) {
		m_entityId = entityId;
	}

	//Getter for entity ID
	unsigned int GetEntityId() {
		return m_entityId;
	}
private:
	unsigned int m_entityId;
};
