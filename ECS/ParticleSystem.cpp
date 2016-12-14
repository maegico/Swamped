#include "ParticleSystem.h"
#include "Game.h"
#include "GlobalFunctions.h"

ParticleSystem::ParticleSystem(unsigned int maxParticles, float particleLifeTime) {
	m_bounds.m_max = XMFLOAT3(100, 100, 100);
	m_bounds.m_min = XMFLOAT3(-100, 0, -100);
	m_particlesPerSecond = maxParticles / particleLifeTime;
	m_particles.resize(maxParticles);
}

ParticleSystem::ParticleSystem() : ParticleSystem(2000000, 20) {

}

ParticleSystem::~ParticleSystem() {

}

size_t ParticleSystem::GetParticleCount() {
	return m_particleCount;
}

vector<Particle> & ParticleSystem::GetParticles() {
	/*vector<Particle> particles;
	particles.resize(m_collapsedCount);
	for (unsigned int c = 0; c < m_collapsedCount; c++) {
		particles[c] = m_collapsedParticles[c].m_component;
	}*/
	//return m_collapsedParticles;
	return m_particles;
}

//void ParticleSystem::Collapse() {
//	m_collapsedCount = 0;
//	m_collapsedParticles.resize(m_particles.count());
//	m_collapsedHandles.resize(m_particles.count());
//	for (unsigned int c = 0; c < m_activeParticles.size(); c++) {
//		if (m_activeParticles[c] == true) {
//			m_collapsedParticles[m_collapsedCount] = m_particles[c];
//			m_collapsedHandles[m_collapsedCount] = c;
//			m_collapsedCount++;
//		}
//	}
//}

//void ParticleSystem::QueueRemove(unsigned int index) {
//	m_removalQueue.add(index);
//}

void ParticleSystem::Update(Game * g, float dt, float totalTime) {
	StartTimer();
	//generate new particles
	unsigned int newParticles = m_particlesPerSecond * dt;
	for (unsigned int c = 0; c < newParticles; c++) {
		if (m_particleIndex >= m_particles.size())
			m_particleIndex = 0;
		Particle & p = m_particles[m_particleIndex];
		p.m_startPosition = XMFLOAT3(fRand(-100, 100), fRand(0, 100), fRand(-100, 100));
		p.m_velocity = XMFLOAT3(fRand(-1, 1), fRand(-2, -1), fRand(-1, 1));
		p.m_birthTime = totalTime;
		float size = fRand(.1f, .15f);
		p.m_size = size;
		++m_particleIndex;
		if (m_particleCount < m_particles.size())
			++m_particleCount;
	}
	//update positions
//#ifdef _DEBUG
//	for (unsigned int c = 0; c < m_collapsedCount; c++) {
//#else
//	parallel_for(size_t(0), m_collapsedCount, [&](unsigned int c) {
//#endif
//		XMVECTOR position;
//		XMVECTOR velocity;
//		auto& cp = m_collapsedParticles[c];
//		auto& p = cp.m_component;
//
//		//load stuff
//		position = XMLoadFloat3(&p.m_startPosition);
//		velocity = XMLoadFloat3(&p.m_velocity);
//		position += dt*velocity;
//
//		//store stuff
//		XMStoreFloat3(&m_particles[cp.m_handle].m_velocity, velocity);
//		XMStoreFloat3(&m_particles[cp.m_handle].m_startPosition, position);
//
//		if (p.m_startPosition.y<0) {
//			QueueRemove(cp.m_handle);
//		}
//#ifdef _DEBUG
//	}
//#else
//	});
//#endif

	StopTimer();
}