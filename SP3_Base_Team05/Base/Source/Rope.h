#ifndef ROPE_H
#define ROPE_H

#include "GameObject.h"

class Rope : public GameObject
{
public:
	Rope();
	~Rope();

	virtual void Init(Vector3 pos, GameObject* target, float lifeTime);
	virtual void Update(double dt);
	virtual void SetupMesh();
	virtual void HandleInteraction(double dt);

private:
	GameObject* target;
	float maxLength;
	float lifetime;

};

Rope* FetchRope();

#endif