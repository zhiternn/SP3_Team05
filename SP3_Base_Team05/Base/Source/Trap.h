#ifndef TRAP_H
#define TRAP_H

#include "GameObject.h"

class Trap : public GameObject
{
public:
	Trap();
	~Trap();

	virtual void Init(Vector3 pos, GameObject* target);
	virtual void Update(double dt);
	virtual void SetupMesh();
	virtual void HandleInteraction(double dt);

private:
	GameObject* target;
	float radius;
	float lifetime;

};

Trap* FetchTrap();

#endif