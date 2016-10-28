#pragma once
#include "ISystem.h"
#include "Component.h"
#include "FreeVector.h"
#include <vector>
#include <map>

template <typename T, typename U>
class PairedSystem : public ISystem {
public:
	void virtual Update(Game * g) {}

	void virtual Create(unsigned int entityId, T tc) {
		auto comp1 = Component<T>(entityId, tc);
		comp1.m_active = true;
		auto comp2 = Component<U>(entityId, {});
		comp2.m_active = true;
		m_handles[entityId] = m_components1.add(comp1);
		m_components2.add(comp2);
	}

	void virtual Create(unsigned int entityId, U uc) {
		auto comp1 = Component<T>(entityId, {});
		comp1.m_active = true;
		auto comp2 = Component<U>(entityId, uc);
		comp2.m_active = true;
		m_handles[entityId] = m_components1.add(comp1);
		m_components2.add(comp2);
	}

	void virtual Create(unsigned int entityId, T tc, U uc) {
		auto comp1 = Component<T>(entityId, tc);
		comp1.m_active = true;
		auto comp2 = Component<U>(entityId, uc);
		comp2.m_active = true;
		m_handles[entityId] = m_components1.add(comp1);
		m_components2.add(comp2);
	}

	void virtual Remove(unsigned int entityId) {
		unsigned int handle = m_handles[entityId];
		m_components1[entityId].m_active = false;
		m_components1.free(handle);
		m_components2[entityId].m_active = false;
		m_components2.free(handle);
		m_handles.erase(entityId);
	}

	T& GetComponent1(unsigned int entityId) {
		return m_components1[m_handles[entityId]].GetData();
	}

	U& GetComponent2(unsigned int entityId) {
		return m_components2[m_handles[entityId]].GetData();
	}
	FreeVector<Component<T>> * GetComponentList1() {
		return &m_components1;
	}
	FreeVector<Component<U>> * GetComponentList2() {
		return &m_components2;
	}
	PairedSystem() {
		m_components1 = FreeVector<Component<T>>();
		m_components2 = FreeVector<Component<U>>();
		m_handles = map<unsigned int, unsigned int>();
	}
	~PairedSystem() {}
protected:
	FreeVector<Component<T>> m_components1;
	FreeVector<Component<U>> m_components2;
private:
	map<unsigned int, unsigned int> m_handles;
};
