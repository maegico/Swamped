#pragma once
#include <ppl.h>
#include <mutex>
#include "GameForwardDecl.h"
#include "FreeVector.h"
#include "ClearVector.h"
#include "Particle.h"
#include "ParticleInput.h"
#include "CollapsedComponent.h"
#include "CollisionComponent.h"

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();
	void Update(Game * g, float dt);
	size_t GetParticleCount();
	vector<ParticleInput> GetParticles();
private:
	void Collapse();
	void QueueRemove(unsigned int index);
	unsigned int m_collapsedCount;
	vector<bool> m_activeParticles;
	FreeVector<Particle> m_particles;
	vector<CollapsedNonComponent<Particle>> m_collapsedParticles;
	ClearVector<unsigned int> m_removalQueue;
	MaxMin m_bounds;
};
