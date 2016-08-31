#ifndef SNAKEMAIN_H
#define SNAKEMAIN_H

#include "SnakeBody.h"
#include "Enemy.h"

#include <vector>

static const float ACTION_TIMER_MIN = 2.0f;
static const float ACTION_TIMER_MAX = 4.0f;
static const int ATTACK_RAM_DAMAGE = 20;
static const float RAGING_SPEED_MULTIPLIER = 3.0f;
static const float BODY_PICKUP_RECOVER_AMOUNT = 80.0f;

class SnakeHead : public Enemy
{
public:
	SnakeHead();
	~SnakeHead();

	virtual void Init(Vector3 pos, unsigned bodyCount);
	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);
	virtual void SetupMesh();

private:
	enum STATE
	{
		STATE_PASSIVE,
		STATE_AGGRESSIVE,

		STATE_END
	};
	
	virtual void Die();
	virtual void Captured();

	void Action();
	void Shoot();
	void Enrage();
	void Recover();

	std::vector<SnakeBody*> bodyList;
	float actionRate;
	float actionLifetime;
	unsigned maxBodyCount;

	bool isRecovering;
	bool isRaging;
};

#endif

//#ifndef SNAKEMAIN_H
//#define SNAKEMAIN_H
//
//#include "SnakeBody.h"
//
//#include "Enemy.h"
//
//static const float ATTACK_TIMER_MIN = 5.0f;
//static const float ATTACK_TIMER_MAX = 8.0f;
//static const int ATTACK_RAM_DAMAGE = 20;
//
//class SnakeHead : public Enemy
//{
//public:
//	SnakeHead();
//	~SnakeHead();
//
//	virtual void Init(Vector3 pos, unsigned bodyCount);
//	virtual void Update(double dt);
//	virtual void HandleInteraction(GameObject* b, double dt);
//	virtual void SetupMesh();
//
//	SnakeBody* GetLast();
//
//private:
//	enum STATE
//	{
//		STATE_PASSIVE,
//		STATE_AGGRESSIVE,
//
//		STATE_END
//	};
//
//	virtual void Die();
//	virtual bool UpdateMovement(double dt);
//
//	void Upgrade();
//	void Action();
//	void Reconnect();
//	void Pull(SnakeBody* body);
//	int GetBodyCount();
//	
//	SnakeBody* backLink;
//	float actionTimer;
//	unsigned maxBodyCount;
//};
//
//#endif