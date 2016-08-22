#ifndef SNAKEMAIN_H
#define SNAKEMAIN_H

#include "SnakeBody.h"

#include "Enemy.h"

class SnakeHead : public Enemy
{
public:
	SnakeHead();
	~SnakeHead();

	virtual void Init(Vector3 pos, unsigned bodyCount);
	virtual void Update(double dt);
	virtual void SetupMesh();

private:
	enum STATE
	{
		STATE_PASSIVE,
		STATE_AGGRESSIVE,

		STATE_END
	};
	void Action();
	void Reconnect();
	void Pull(SnakeBody* body);
	virtual void Die();

	SnakeBody* backLink;

	float actionTimer;
	
};

#endif