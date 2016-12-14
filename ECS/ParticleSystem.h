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
	ParticleSystem(unsigned int maxParticles, float particleLifetime);
	ParticleSystem();
	~ParticleSystem();
	void Update(Game * g, float dt, float totalTime);
	size_t GetParticleCount();
	vector<Particle> & GetParticles();
private:
	vector<Particle> m_particles;
	MaxMin m_bounds;
	float m_particlesPerSecond;
	unsigned int m_particleIndex;
	unsigned int m_particleCount;
};
