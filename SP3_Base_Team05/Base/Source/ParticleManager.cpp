#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
	for (int i = 0; i < P_END; ++i)
	{
		if (particle[i])
			delete particle[i];

		particle[i] = NULL;
	}
}

ParticleManager::~ParticleManager()
{
	for (int i = 0; i < P_END; ++i)
	{
		if (particle[i])
			delete particle[i];
	}
}

void ParticleManager::GenerateParticle(
	Mesh* mesh,
	PARTICLE_EFFECT type,
	Particle::PARTICLE_PATTERN pattern,
	Particle::ENTRY_EFFECT entryEffect,
	float entrySpeed,
	Particle::EXIT_EFFECT exitEffect,
	float exitSpeed
	)
{
	if (!mesh)
		return;

	if (ParticleManager::GetInstance().particle[type])
		delete ParticleManager::GetInstance().particle[type];

	ParticleManager::GetInstance().particle[type] = new Particle();
	ParticleManager::GetInstance().particle[type]->pattern = pattern;
	ParticleManager::GetInstance().particle[type]->entryEffect = entryEffect;
	ParticleManager::GetInstance().particle[type]->entrySpeed = entrySpeed;
	ParticleManager::GetInstance().particle[type]->exitEffect = exitEffect;
	ParticleManager::GetInstance().particle[type]->exitSpeed = exitSpeed;
}