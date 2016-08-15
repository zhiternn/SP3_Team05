#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"

class Particle
{
public:
	enum ENTRY_EFFECT
	{
		ENTRY_NONE,

		ENTRY_SCALEIN,

		ENTRY_END
	};
	enum EXIT_EFFECT
	{
		EXIT_NONE,
		
		EXIT_SCALEOUT,
		
		EXIT_END
	};

	Particle();
	~Particle();

	virtual void Update(double dt);

	void ScaleIn(double dt);
	void ScaleOut(double dt);

	Vector3 pos;
	Vector3 vel;
	Vector3 scale;

	float rotation;
	float rotationSpeed;
	
	bool active;
	float lifetime;

	bool entered;
	bool exited;
	float entrySpeed;
	float exitSpeed;
	Vector3 entryTarget;
	Vector3 exitTarget;
	ENTRY_EFFECT entryEffect;
	EXIT_EFFECT exitEffect;
};

#endif