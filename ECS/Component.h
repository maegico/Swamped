#pragma once

//Wrapper for component data
template <typename T> class Component {
public:
	//Whether the component is active
	bool m_active = false;
	Component() {}
	Component(unsigned int entityId, T comp) {
		m_entityId = entityId;
		m_data = comp;
	}

	//Getter for component data
	T& GetData() {
		return m_data;
	}

	//Getter for entity ID
	unsigned int GetEntityId() {
		return m_entityId;
	}
private:
	unsigned int m_entityId;
	T m_data;
};
