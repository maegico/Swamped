#pragma once
#include "SystemBase.h"
#include "ComponentData.h"
#include "FreeVector.h"
#include <vector>
#include <map>

using namespace std;

//A system for a single component
template <typename T>
class System : public SystemBase {
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

	//Returns a pointer to the component list
	FreeVector<T> * GetComponentList() {
		return &m_components;
	}	

	//Gets the number of components
	size_t GetCount() {
		return m_components1.count();
	}

	//Initializes m_components and m_handles
	System() : SystemBase() {
		m_components = FreeVector<T>();
		m_handles = map<unsigned int, unsigned int>();
	}
	~System(){}
protected:
	//Holds components
	FreeVector<T>  m_components;
private:
	//Holds entityId - index pairs
	map<unsigned int, unsigned int> m_handles;
};