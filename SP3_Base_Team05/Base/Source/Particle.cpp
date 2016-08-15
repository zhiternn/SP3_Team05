#include "Particle.h"

Particle::Particle() :
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
	scale.x += entryTarget.x * dt * entrySpeed;
	scale.y += entryTarget.y * dt * entrySpeed;
	scale.z += entryTarget.z * dt * entrySpeed;

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
	scale.x -= exitTarget.x * dt * exitSpeed;
	scale.y -= exitTarget.y * dt * exitSpeed;
	scale.z -= exitTarget.z * dt * exitSpeed;

	if (
		scale.x <= exitTarget.x &&
		scale.y <= exitTarget.y &&
		scale.z <= exitTarget.z
		)
	{
		exited = true;
	}
}