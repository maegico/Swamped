#pragma once
#include "ISystem.h"
#include "Component.h"
#include "FreeVector.h"
#include <vector>
#include <map>

using namespace std;

template <typename T>
class System : public ISystem {
public:
	void virtual Update(Game * g) {}

	//Creates a component of type T and adds it to m_components and m_handles
	void Create(unsigned int entityId, T tc) {
		auto comp = Component<T>(entityId, tc);
		comp.m_active = true;
		//FreeVector::add returns the index of the added element
		m_handles[entityId] = m_components.add(comp);
	}

	//Deactivates the component with the given ID, frees the space in m_components, and erases the handle
	void Remove(unsigned int entityId) {
		m_components[entityId].m_active = false;
		m_components.free(m_handles[entityId]);
		m_handles.erase(m_handles.erase(entityId));
	}

	//Returns a reference to the component with the given ID
	T& GetComponent(unsigned int entityId) {
		return m_components[m_handles[entityId]].GetData();
	}

	//Initializes m_components and m_handles
	System() {
		m_components = FreeVector<Component<T>>();
		m_handles = map<unsigned int, unsigned int>();
	}
	~System(){}
protected:
	//Holds component data
	FreeVector<Component<T>>  m_components;
private:
	//Holds entityId - index pairs
	map<unsigned int, unsigned int> m_handles;
};