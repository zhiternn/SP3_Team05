#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"
#include "Mesh.h"

class Particle
{
public:
	enum ENTRY_EFFECT
	{
		ENTRY_NONE,

		ENTRY_SCALEIN,

		ENTRY_END
	};
	enum PARTICLE_PATTERN
	{
		PATTERN_NONE,

		PATTERN_SPRAY,
		PATTERN_DISPERSE,

		PATTERN_END
	};
	enum EXIT_EFFECT
	{
		EXIT_NONE,
		
		EXIT_SCALEOUT,
		
		EXIT_END
	};

	Particle();
	~Particle();

	virtual void Init(Vector3 pos, Vector3 vel);
	virtual void Update(double dt);

	void ScaleIn(double dt);
	void ScaleOut(double dt);

	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	PARTICLE_PATTERN pattern;

	float rotation;
	float rotationSpeed;
	
	bool active;
	float lifetime;
	bool isPhysics;

	bool entered;
	bool exited;
	float entrySpeed;
	float exitSpeed;
	Vector3 entryTarget;
	ENTRY_EFFECT entryEffect;
	EXIT_EFFECT exitEffect;
};

#endif