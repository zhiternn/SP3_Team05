#ifndef SNAKEMAIN_H
#define SNAKEMAIN_H

#include "SnakeBody.h"

#include "Enemy.h"

static const float ATTACK_TIMER_MIN = 5.0f;
static const float ATTACK_TIMER_MAX = 8.0f;
static const int ATTACK_RAM_DAMAGE = 20;

class SnakeHead : public Enemy
{
public:
	SnakeHead();
	~SnakeHead();

	virtual void Init(Vector3 pos, unsigned bodyCount);
	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);
	virtual void SetupMesh();

	SnakeBody* GetLast();

private:
	enum STATE
	{
		STATE_PASSIVE,
		STATE_AGGRESSIVE,

		STATE_END
	};

	virtual void Die();
	virtual bool UpdateMovement(double dt);

	void Upgrade();
	void Action();
	void Reconnect();
	void Pull(SnakeBody* body);
	int GetBodyCount();
	
	SnakeBody* backLink;
	float actionTimer;
	unsigned maxBodyCount;
};

#endif