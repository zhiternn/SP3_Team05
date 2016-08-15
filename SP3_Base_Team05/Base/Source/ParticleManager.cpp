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

	if (particle[type])
		delete particle[type];

	particle[type] = new Particle();
	particle[type]->pattern = pattern;
	particle[type]->entryEffect = entryEffect;
	particle[type]->entrySpeed = entrySpeed;
	particle[type]->exitEffect = exitEffect;
	particle[type]->exitSpeed = exitSpeed;
}