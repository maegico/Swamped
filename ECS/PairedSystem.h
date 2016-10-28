#pragma once
#include "ISystem.h"
#include "ComponentData.h"
#include "FreeVector.h"
#include <vector>
#include <map>

template <typename T, typename U>
class PairedSystem : public ISystem {
public:
	void virtual Update(Game * g, float dT) {}

	void virtual Create(unsigned int entityId, T tc, U uc) {
		ComponentData comp = ComponentData(entityId);
		comp.m_active = true;
		unsigned int index = m_components1.add(tc);
		m_handles[entityId] = index;
		m_components2.add(uc);
		if (index == m_componentData.size())
			m_componentData.push_back(comp);
		else
			m_componentData[index] = comp;
	}

	void virtual Remove(unsigned int entityId) {
		unsigned int handle = m_handles[entityId];
		m_componentData[handle].m_active = false;
		m_components1.free(handle);
		m_components2.free(handle);
		m_handles.erase(entityId);
	}

	T& GetComponent1(unsigned int entityId) {
		return m_components1[m_handles[entityId]];
	}

	U& GetComponent2(unsigned int entityId) {
		return m_components2[m_handles[entityId]];
	}
	FreeVector<T> * GetComponentList1() {
		return &m_components1;
	}
	FreeVector<U> * GetComponentList2() {
		return &m_components2;
	}
	PairedSystem() {
		m_componentData = vector<ComponentData>();
		m_components1 = FreeVector<T>();
		m_components2 = FreeVector<U>();
		m_handles = map<unsigned int, unsigned int>();
	}
	~PairedSystem() {}
protected:
	FreeVector<T> m_components1;
	FreeVector<U> m_components2;
	vector<ComponentData> m_componentData;
private:
	map<unsigned int, unsigned int> m_handles;
};
