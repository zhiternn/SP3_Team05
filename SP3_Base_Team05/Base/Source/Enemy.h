#ifndef ENEMY_H
#define	ENEMY_H

#include <stack>
#include "Vector3.h"
#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();


	void UpdateMovement(double dt);
	void AddDestination(Vector3 pos);

private:
	std::stack<Vector3> destinations;
	bool Reached(Vector3 pos);

};

Enemy* FetchEnemy();

#endif // !ENEMY_H
