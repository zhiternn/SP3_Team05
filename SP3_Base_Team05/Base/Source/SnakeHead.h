#ifndef SNAKEMAIN_H
#define SNAKEMAIN_H

#include "SnakeBody.h"

#include "Enemy.h"

class SnakeHead : public Enemy
{
public:
	SnakeHead();
	~SnakeHead();

	virtual void Init(Vector3 pos);
	virtual void Update(double dt);

private:
	void Pull(SnakeBody* body);
	SnakeBody* back;
	
};

#endif