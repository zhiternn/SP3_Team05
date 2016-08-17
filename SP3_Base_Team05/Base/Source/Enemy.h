#ifndef ENEMY_H
#define	ENEMY_H

#include <stack>
#include "Vector3.h"
#include "Entity.h"

static const float ENEMY_MOVEMENT_SPEED = 200.0f;
static const float REACH_CHECKER = 5.0f;

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();

	void Init(Vector3 pos);
	void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);

	void SetRate(float rate);

	float GetRate();

protected:
	enum DESTINATION_PRIORITY
	{
		D_MAIN,
		D_ENVIRONMENT,
		D_DODGE,

		D_END
	};

	void AddDestination(Vector3 pos);
	bool UpdateMovement(double dt);
	bool Reached(Vector3 pos);

	std::stack<Vector3> destinations;

	float checkReached;
	float speedLimit;

	float captureRatio;
};

Enemy* FetchEnemy();

#endif // !ENEMY_H
