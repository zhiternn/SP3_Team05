#ifndef ROPE_H
#define ROPE_H

#include "GameObject.h"

class Rope : public GameObject
{
public:
	Rope();
	~Rope();

	virtual void Init(Vector3 pos, GameObject* target);
	virtual void Update(double dt);
	virtual void HandleInteraction(double dt);

private:
	GameObject* target;
	float maxLength;

};

Rope* FetchRope();

#endif