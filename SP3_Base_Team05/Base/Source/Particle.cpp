#include "Particle.h"

Particle::Particle(PARTICLE_TYPE typeValue) :
type(typeValue),
pos(0, 0, 0),
scale(1, 1, 1),
rotation(0),
rotationSpeed(0),
lifetime(0.0f),
active(false),
entered(false),
exited(false)
{
}

Particle::~Particle()
{
}

void Particle::ScaleIn(Vector3 target, float speed)
{
	scale.Lerp(target, speed);

	if (
		scale.x >= target.x &&
		scale.y >= target.z &&
		scale.z >= target.z
		)
	{
		entered = true;
	}
}

void Particle::ScaleOut(Vector3 target, float speed)
{
	scale.x -= speed;
	scale.y -= speed;
	scale.z -= speed;

	if (
		scale.x <= target.x &&
		scale.y <= target.z &&
		scale.z <= target.z
		)
	{
		exited = true;
	}
}