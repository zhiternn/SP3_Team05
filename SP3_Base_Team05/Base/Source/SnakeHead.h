#ifndef SNAKEMAIN_H
#define SNAKEMAIN_H

#include "Enemy.h"

class SnakeHead : public Enemy
{
public:
	SnakeHead();
	~SnakeHead();

	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);

	void SetTarget(Entity* target);

private:
	Entity* target;
};

#endif