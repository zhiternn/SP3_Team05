#include "Particle.h"

Particle::Particle() :
mesh(NULL),
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
	if (mesh)
		delete mesh;
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