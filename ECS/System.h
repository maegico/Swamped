#pragma once
#include "ISystem.h"
#include "ComponentData.h"
#include "FreeVector.h"
#include <vector>
#include <map>

using namespace std;

template <typename T>
class System : public ISystem {
public:
	void virtual Update(Game * g, float dT) {}

	//Creates a component of type T and adds it to m_components and m_handles
	void virtual Create(unsigned int entityId, T tc) {
		auto comp = ComponentData(entityId);
		comp.m_active = true;
		//FreeVector::add returns the index of the added element
		unsigned int index = m_components.add(tc);
		m_handles[entityId] = index;
		if (index == m_componentData.size())
			m_componentData.push_back(comp);
		else
			m_componentData[index] = comp;
	}

	//Deactivates the component with the given ID, frees the space in m_components, and erases the handle
	void virtual Remove(unsigned int entityId) {
		unsigned int index = m_handles[entityId];
		m_componentData[index].m_active = false;
		m_components.free(index);
		m_handles.erase(entityId);
	}

	//Returns a reference to the component with the given ID
	T& GetComponent(unsigned int entityId) {
		return m_components[m_handles[entityId]];
	}

	FreeVector<T> * GetComponentList() {
		return &m_components;
	}

	bool SearchForEntityId(unsigned int& transformIndex, unsigned int entityId) {
		unsigned int total = 0;
		while (tcds[transformIndex].GetEntityId() != entityId)
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

	//Initializes m_components and m_handles
	System() {
		m_componentData = vector<ComponentData>();
		m_components = FreeVector<T>();
		m_handles = map<unsigned int, unsigned int>();
	}
	~System(){}
protected:
	//Holds component data
	FreeVector<T>  m_components;
	vector<ComponentData> m_componentData;
private:
	//Holds entityId - index pairs
	map<unsigned int, unsigned int> m_handles;
};