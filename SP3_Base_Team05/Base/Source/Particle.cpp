#include "Particle.h"
#include "MeshManager.h"

std::vector<Particle*> Particle::particleList;

Particle::Particle():
rotation(0.0f),
rotationSpeed(0.0f),
active(false),
lifetime(0.0f),
isPhysics(false),
hasDirection(false),
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

void Particle::Init(Vector3 pos, Vector3 vel, float rotationSpeed)
{
	this->pos = pos;
	this->vel = vel;
	this->rotationSpeed = rotationSpeed;
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
				mesh = NULL;
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

void Particle::SetupMesh()
{
	modelStack.Translate(this->pos.x, this->pos.y, this->pos.z + 0.01f);
	if (hasDirection)
	{
		float degree = Math::RadianToDegree(atan2(this->vel.y, this->vel.x));
		modelStack.Rotate(degree, 0, 0, 1);
	}
	else
	{
		modelStack.Rotate(rotation, 0, 0, 1);
	}
	modelStack.Scale(this->scale.x, this->scale.y, this->scale.z);
}

Particle* FetchParticle()
{
	for (std::vector<Particle*>::iterator it = Particle::particleList.begin(); it != Particle::particleList.end(); ++it)
	{
		if ((*it)->active == false)
		{
			(*it)->active = true;
			return (*it);
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		Particle::particleList.push_back(new Particle());
	}

	(*(Particle::particleList.end() - 10))->active = true;
	return (*(Particle::particleList.end() - 10));
}

void EmitDashParticle(Vector3 pos, Vector3 dir)
{
	Particle* particle = FetchParticle();
	particle->mesh = meshList[GEO_PARTICLE_SMOKE];

	pos.z += 0.01f;
	particle->Init(pos, dir);
	particle->lifetime = 0.1f;
	particle->scale.Set(0.1f, 0.1f, 0.1f);
	particle->hasDirection = true;
	particle->entered = false;
	particle->entrySpeed = 100.0f;
	particle->entryEffect = Particle::ENTRY_SCALEIN;
	particle->entryTarget.Set(20.0f, 20.0f, 20.0f);
}

void EmitHitParticle(Vector3 pos, float size)
{
	Particle* particle = FetchParticle();
	particle->mesh = meshList[GEO_PARTICLE_HIT];

	particle->Init(pos);
	particle->lifetime = 0.0f;
	particle->scale.Set(0.1f, 0.1f, 0.1f);
	particle->entered = false;
	particle->entrySpeed = 100.0f;
	particle->entryEffect = Particle::ENTRY_SCALEIN;
	particle->entryTarget.Set(size, size, size);
}

void EmitSignalParticle(Vector3 pos, float initialScale, float size)
{
	Particle* particle = FetchParticle();
	particle->mesh = meshList[GEO_PARTICLE_SIGNAL];

	particle->Init(pos);
	particle->lifetime = 0.0f;
	particle->scale.Set(initialScale, initialScale, initialScale);
	particle->entered = false;
	particle->entrySpeed = 80.0f;
	particle->entryEffect = Particle::ENTRY_SCALEIN;
	particle->entryTarget.Set(size, size, size);
}

void EmitSkidParticle(Vector3 pos, float size, Vector3 dir)
{
	Particle* particle = FetchParticle();
	particle->mesh = meshList[GEO_PARTICLE_SKID];

	particle->Init(pos, dir * 0.1f);
	particle->hasDirection = true;
	particle->lifetime = 4.0f;
	particle->scale.Set(size, size, size);
}

void EmitTeleportParticle(Vector3 pos, float size)
{
	Particle* particle = FetchParticle();
	particle->mesh = meshList[GEO_PARTICLE_TELEPORT];

	particle->Init(pos);
	particle->lifetime = 0.5f;
	particle->scale.Set(0.1f, 0.1f, 0.1f);
	particle->entered = false;
	particle->entrySpeed = 500.f;
	particle->entryEffect = Particle::ENTRY_SCALEIN;
	particle->entryTarget.Set(size, size, size);
}