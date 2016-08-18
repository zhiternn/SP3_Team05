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

	//Setters
	void SetTarget(Entity* target);
	void SetRate(float rate);
	//Getters
	float GetRate();

	std::vector<Vector3> destinations;

protected:
	enum DESTINATION_PRIORITY
	{
		D_MAIN,
		D_ENVIRONMENT,
		D_DODGE,

		D_END
	};

	void AddDestination(Vector3 pos);
	bool UpdateMovement(double dt);//returns false if out of waypoints
	bool Reached(Vector3 pos);
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
