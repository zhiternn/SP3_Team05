#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

static const float MOVEMENT_SPEED = 50.0f;
static const float MOVEMENT_LIMIT = 15.f;

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Init(Vector3 pos, Vector3 scale, Vector3 front);
	void UpdateInputs(double dt);
	void Update(double dt);

private:

	bool isMoving;
	bool isDashing;
};


#endif