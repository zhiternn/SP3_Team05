#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "Particle.h"
#include "Vector3.h"

#include <vector>

using std::vector;

static const Vector3 GRAVITY = Vector3(0, 0, -9.8f);

class ParticleManager
{
	enum PARTICLE_EFFECT
	{
		P_HIT,

		P_END,
	};
public:
	static ParticleManager& GetInstance()
	{
		static ParticleManager pm;
		return pm;
	}

	static vector<Particle> particleList;

	static void GenerateParticle(
		Mesh* mesh,
		PARTICLE_EFFECT type,
		Particle::PARTICLE_PATTERN pattern = Particle::PATTERN_NONE,
		Particle::ENTRY_EFFECT entryEffect = Particle::ENTRY_NONE,
		float entrySpeed = 0.0f,
		Particle::EXIT_EFFECT exitEffect = Particle::EXIT_NONE,
		float exitSpeed = 0.0f
		);

	~ParticleManager();

private:
	ParticleManager();

	static Particle *particle[P_END];
};

#endif