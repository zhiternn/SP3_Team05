#ifndef TRAP_H
#define TRAP_H

#include "GameObject.h"

class Trap : public GameObject
{
public:
	Trap();
	~Trap();

	virtual void Init(Vector3 pos);
	virtual void Update(double dt);
	virtual void SetupMesh();
	virtual void HandleInteraction(GameObject* b, double dt);

private:
	float radius;
	float lifetime;
	bool isTrapped;

};

Trap* FetchTrap();

#endif