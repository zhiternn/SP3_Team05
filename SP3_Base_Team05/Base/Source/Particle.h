#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"
#include "Mesh.h"

#include <vector>

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

	static std::vector<Particle*> particleList;

	virtual void Init(Vector3 pos, Vector3 vel = Vector3(), float rotationSpeed = 0.0f);
	virtual void Update(double dt);
	virtual void SetupMesh();

	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Mesh* mesh;

	float rotation;
	float rotationSpeed;
	
	bool active;
	float lifetime;
	bool isPhysics;
	bool hasDirection;

	bool entered;
	bool exited;
	float entrySpeed;
	float exitSpeed;
	Vector3 entryTarget;
	ENTRY_EFFECT entryEffect;
	EXIT_EFFECT exitEffect;

protected:
	void ScaleIn(double dt);
	void ScaleOut(double dt);
};

Particle* FetchParticle();

void EmitDashParticle(Vector3 pos, Vector3 dir);

#endif