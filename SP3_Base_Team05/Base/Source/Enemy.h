#ifndef ENEMY_H
#define	ENEMY_H

#include <stack>
#include "Vector3.h"
#include "GameObject.h"

static const float ENEMY_MOVEMENT_SPEED = 20.0f;
static const float REACH_CHECKER = 5.0f;

class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();

	void Init(Vector3 pos, unsigned int HP);
	void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);

	void UpdateMovement(double dt);
	void AddDestination(Vector3 pos);

	std::stack<Vector3> destinations;

	//Damage Functions
	unsigned int GetHP();
	void TakeDamage(unsigned int dmg);

private:
	//std::stack<Vector3> destinations;
	bool Reached(Vector3 pos);
	float checkReached;
	float speedLimit;

	unsigned int enemyHP;
};

Enemy* FetchEnemy();

#endif // !ENEMY_H
