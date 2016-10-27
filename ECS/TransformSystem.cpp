#include "TransformSystem.h"
#include "Constructors.h"

void TransformSystem::Update(Game * g) {
	for (unsigned int c = 0; c < m_components.size(); c++) {
		if (rand() % 2 == 1)
			g->RemoveEntity(m_components[c].GetEntityId());
	}
	unsigned int newComponents = rand() % 1000;
	for (unsigned int c = 0; c < newComponents; c++)
	{
		Constructors::CreateTransform(g, {});
	}
}