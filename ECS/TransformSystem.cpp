#include "TransformSystem.h"
#include "Constructors.h"

void TransformSystem::Update(Game * g) {
	for (unsigned int c = 0; c < m_components.size(); c++) {
		if (m_components[c].m_active && rand() % 2 == 1)
			g->RemoveEntity(m_components[c].GetEntityId());
	}
	unsigned int newComponents = rand() % 10000;
	for (unsigned int c = 0; c < newComponents; c++)
	{
		Constructors::CreateTransform(g, {});
	}
}

size_t TransformSystem::GetSize() {
	return m_components.size();
}

size_t TransformSystem::GetCount() {
	return m_components.count();
}