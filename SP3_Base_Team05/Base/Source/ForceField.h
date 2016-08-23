#ifndef FORCEFIELD_H
#define FORCEFIELD_H

#include "GameObject.h"

class ForceField : public GameObject
{
public:
	ForceField();
	~ForceField();

	virtual void Init(Vector3 pos, GameObject* target, float lifeTime);
	virtual void Update(double dt);
	virtual void SetupMesh();
	virtual void HandleInteraction(double dt);

private:
	GameObject* target;
	float maxRadius;
	float lifetime;

};

//ForceField* FetchRope();

#endif