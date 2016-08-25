#ifndef ENEMY_H
#define	ENEMY_H

#include "Vector3.h"
#include "Entity.h"
#include "Weapon.h"

static const float REACH_CHECKER = 2.0f;
static const float CAPTURE_GOAL = 5.f;

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();

	virtual void Init(Vector3 pos);
	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);
	virtual void Capturing(float rate);
	virtual void Captured();

	//Setters
	void SetTarget(Entity* target);
	void SetRate(float rate);
	//Getters
	float GetRate();
	bool Reached(Vector3 pos);

	float speedLimit;
	float movementSpeed;

    void SetSpeedLimit(float speed);
    void SetMovementSpeed(float speed);
    float GetSpeedLimit();
    float GetMovementSpeed();

	float captureRate;

protected:
	enum MOVEMENT_PRIORITY
	{
		MOVETO_TARGET,
		MOVETO_AVOID_ENVIRONMENT,
		MOVETO_AVOID_PROJECTILE,

		MOVETO_END,
	};

	void ChangeDestination(MOVEMENT_PRIORITY priority, Vector3 pos);
	bool UpdateMovement(double dt);//returns false if out of waypoints
	Vector3 FindNewPath(Vector3 destination, GameObject* obstacle);

	std::vector<Vector3> destinations;
	float destinationCountdown;
	Entity* target;

	//float captureRate;
	bool isCaptured;
};

Enemy* FetchEnemy();

#endif // !ENEMY_H
