#ifndef ENEMY_H
#define	ENEMY_H

#include "Vector3.h"
#include "Entity.h"
#include "Weapon.h"

static const float REACH_CHECKER = 2.0f;

class Enemy : public Entity
{
public:
	enum MOVEMENT_PRIORITY
	{
		MOVETO_TARGET,
		MOVETO_AVOID_ENVIRONMENT,
		MOVETO_AVOID_PROJECTILE,

		MOVETO_END,
	};

	Enemy();
	~Enemy();

	virtual void Init(Vector3 pos);
	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);
	bool UpdateMovement(double dt);//returns false if out of waypoints
	void ChangeDestination(MOVEMENT_PRIORITY priority, Vector3 pos);

	//Setters
	void SetTarget(Entity* target);
	void SetRate(float rate);
	void SetSpeedLimit(float speed);
	void SetMovementSpeed(float speed);
	void SetCaptureRate(float captureRate);
	void SetCaptured(bool isCaptured);
	void SetCapturing(bool isCapturing);

	//Getters
	float GetRate();
	bool Reached(Vector3 pos);
	float GetSpeedLimit();
	float GetMovementSpeed();
	float GetCaptureRate();
	bool IsCaptured();
	bool IsCapturing();

protected:

	Vector3 FindNewPath(Vector3 destination, GameObject* obstacle);
	void Captured();
	void Capturing(double dt);

	std::vector<Vector3> destinations;
	float destinationCountdown;
	Entity* target;

	float speedLimit;
	float movementSpeed;

	float captureRate;
	bool isCaptured;
	bool isCapturing;
};

Enemy* FetchEnemy();

#endif // !ENEMY_H
