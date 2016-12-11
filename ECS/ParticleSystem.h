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
#include "Timeable.h"

class ParticleSystem : public Timeable{
public:
	ParticleSystem();
	~ParticleSystem();
	void Update(Game * g, float dt, float totalTime);
	size_t GetParticleCount();
	vector<Particle> & GetParticles();
private:
	void Collapse();
	void QueueRemove(unsigned int index);
	unsigned int m_collapsedCount;
	vector<bool> m_activeParticles;
	FreeVector<Particle> m_particles;
	vector<unsigned int> m_collapsedHandles;
	vector<Particle> m_collapsedParticles;
	ClearVector<unsigned int> m_removalQueue;
	MaxMin m_bounds;
};
