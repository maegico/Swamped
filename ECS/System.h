#pragma once
#include "SystemBase.h"
#include "ComponentData.h"
#include "CollapsedComponent.h"
#include "FreeVector.h"
#include "EntityIdTypeDef.h"
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

//A system for a single component
template <typename T>
class System : public SystemBase {
public:
	void virtual Update(Game * g, float dT) = 0;

	//Creates a component of type T and adds it to m_components and m_handles
	unsigned int virtual Create(EntityId entityId, T tc) {
		ComponentData comp = { true, entityId };
		//FreeVector::add returns the index of the added element
		unsigned int index = m_components.add(tc);
		m_handles[entityId] = index;
		if (index == m_componentData.size())
			m_componentData.push_back(comp);
		else
			m_componentData[index] = comp;
		return index;
	}

	//Deactivates the component with the given ID, frees the space in m_components, and erases the handle
	void virtual Remove(EntityId entityId) {
		unsigned int index = m_handles[entityId];
		m_componentData[index].m_active = false;
		m_components.free(index);
		m_handles.erase(entityId);
	}

	void Collapse() {
		m_collapsedCount = 0;
		m_collapsedComponents.resize(GetCount());
		//m_collapsedEntityIds.resize(GetCount());
		//m_collapsedHandles.resize(GetCount());
		for (unsigned int c = 0; c < m_componentData.size(); c++)
			if (m_componentData[c].m_active) {
				m_collapsedComponents[m_collapsedCount] = { m_components[c], m_componentData[c].m_entityId, c };
				//m_collapsedEntityIds[m_collapsedCount] = m_componentData[c].GetEntityId();
				//m_collapsedHandles[m_collapsedCount] = c;
				m_collapsedCount++;
			}
	}

	//Returns a reference to the component with the given ID
	T& GetComponent(EntityId entityId) {
		return m_components[m_handles[entityId]];
	}

	//Returns a pointer to the component list
	FreeVector<T> * GetComponentList() {
		return &m_components;
	}	

	//Gets the number of components
	size_t GetCount() {
		return m_components.count();
	}

	//Initializes m_components and m_handles
	System() : SystemBase() {
		//m_components = FreeVector<T>();
		//m_collapsedComponents = vector<CollapsedComponent<T>>();
		//m_collapsedEntityIds = vector<unsigned int>();
		//m_collapsedHandles = vector<unsigned int>();
		//m_handles = unordered_map<unsigned int, unsigned int>();
	}
	~System(){}
protected:
	//Holds components
	FreeVector<T>  m_components;
	vector<CollapsedComponent<T>> m_collapsedComponents;
	//vector<unsigned int> m_collapsedEntityIds;
	//vector<unsigned int> m_collapsedHandles;
	unsigned int m_collapsedCount = 0;
private:
	//Holds entityId - index pairs
	unordered_map<EntityId, unsigned int> m_handles;
};