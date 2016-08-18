#ifndef SNAKEBODY_H
#define SNAKEBODY_H

#include "Enemy.h"

class SnakeBody : public Enemy
{
public:
	SnakeBody();
	~SnakeBody();

	virtual void Update(double dt);
	void LinkTo(SnakeBody* entity);

	void Goto(Vector3 pos);

private:
	void Pull(SnakeBody* body);
	SnakeBody* back;

};

#endif