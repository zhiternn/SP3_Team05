#include "Particle.h"

Particle::Particle():
pattern(PATTERN_NONE),
rotation(0.0f),
rotationSpeed(0.0f),
active(false),
lifetime(0.0f),
isPhysics(false),
entered(false),
exited(false),
entrySpeed(1.0f),
exitSpeed(1.0f),
entryEffect(ENTRY_NONE),
exitEffect(EXIT_NONE)
{
}

Particle::~Particle()
{
}

void Particle::Init(Vector3 pos, Vector3 vel)
{
	switch (pattern)
	{
	case PATTERN_SPRAY:
		break;
	case PATTERN_DISPERSE:
		break;

	default:break;
	}
}

void Particle::Update(double dt)
{
	if (!entered)
	{
		switch (entryEffect)
		{
		case Particle::ENTRY_SCALEIN:
			ScaleIn(dt);
			break;

		default:
			entered = true;
			break;
		}
	}
	else
	{
		if (lifetime > 0)
		{
			pos += vel * (float)dt;
			rotation += rotationSpeed * dt;
			lifetime -= (float)dt;
		}
		else
		{
			if (!exited)
			{
				switch (exitEffect)
				{
				case Particle::EXIT_SCALEOUT:
					ScaleOut(dt);
					break;

				default:
					exited = true;
					break;
				}
			}
			else
			{
				active = false;
			}
		}
	}
}

void Particle::ScaleIn(double dt)
{
	scale.x += dt * entrySpeed;
	scale.y += dt * entrySpeed;
	scale.z += dt * entrySpeed;

	if (
		scale.x >= entryTarget.x &&
		scale.y >= entryTarget.y &&
		scale.z >= entryTarget.z
		)
	{
		entered = true;
	}
}

void Particle::ScaleOut(double dt)
{
	scale.x -= dt * exitSpeed;
	scale.y -= dt * exitSpeed;
	scale.z -= dt * exitSpeed;

	if (
		scale.x <= 0.1f &&
		scale.y <= 0.1f &&
		scale.z <= 0.1f
		)
	{
		exited = true;
	}
}