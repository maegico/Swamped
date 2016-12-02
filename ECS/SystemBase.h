#pragma once
#include "ISystem.h"
#include <vector>
using namespace std;
class SystemBase : public ISystem {
public:
	void virtual Update(Game * g, float dT) = 0;
	void virtual Remove(EntityId entityId) = 0;
	//Returns a reference to the ComponentData vector
	vector<ComponentData> & GetComponentData() {
		return m_componentData;
	}

	//Increments the given index until it matches the given entityID
	//Wraps the end of the collection and stops after looping the length of m_componentData
	//Returns true on success and false on fail
	bool SearchForEntityId(unsigned int& transformIndex, EntityId entityId) {
		unsigned int total = 0;
		while (m_componentData[transformIndex].m_entityId != entityId)
		{
			transformIndex++;
			total++;
			if (transformIndex == m_componentData.size())
				transformIndex = 0;
			if (total == m_componentData.size())
				return false;
		}
		return true;
	}

	//Gets the total capacity of the system
	size_t GetSize() {
		return m_componentData.size();
	}

	SystemBase() {
		m_componentData = vector<ComponentData>();
	}
	~SystemBase() {}
protected:
	//Holds component bookkeeping data
	vector<ComponentData> m_componentData;
private:	
};