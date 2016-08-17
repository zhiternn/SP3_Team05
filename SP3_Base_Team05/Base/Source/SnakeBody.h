#ifndef SNAKEBODY_H
#define SNAKEBODY_H

#include "Enemy.h"

class SnakeBody : public Enemy
{
public:
	SnakeBody();
	~SnakeBody();

	virtual void Update(double dt);

	void LinkTo(Entity* entity);

private:
	Entity* link;

};

#endif