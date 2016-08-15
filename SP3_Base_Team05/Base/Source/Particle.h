#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"

enum PARTICLE_TYPE
{
	P_ARROW = 0,
	P_FIRE,

	P_TOTAL
};

class Particle
{
public:
	Particle(PARTICLE_TYPE typeValue = P_ARROW);
	~Particle();

	void ScaleIn(Vector3 target, float speed);
	void ScaleOut(Vector3 target, float speed);

	PARTICLE_TYPE type;

	Vector3 pos;
	Vector3 vel;
	Vector3 scale;

	float rotation;
	float rotationSpeed;

	float lifetime;

	bool active;

	bool entered;
	bool exited;
};

#endif