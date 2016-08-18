#ifndef ENEMY_H
#define	ENEMY_H

#include "Vector3.h"
#include "Entity.h"

static const float REACH_CHECKER = 2.0f;

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();

	virtual void Init(Vector3 pos);
	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);

	//Setters
	void SetTarget(Entity* target);
	void SetRate(float rate);
	//Getters
	float GetRate();
	bool Reached(Vector3 pos);

	std::vector<Vector3> destinations;

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

	// moved to public for testing
	//std::vector<Vector3> destinations;
	float destinationCountdown;

	Entity* target;
	float speedLimit;
	float movementSpeed;
	float captureRatio;
};

Enemy* FetchEnemy();

#endif // !ENEMY_H
