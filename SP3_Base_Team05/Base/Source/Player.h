#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Weapon.h"

static const float MOVEMENT_SPEED = 80.0f;
static const float MOVEMENT_LIMIT = 30.f;
static const float DASH_COOLDOWN = 2.f;

class Player : public GameObject
{
public:
	Player();
	~Player();

	// Functions
	void Init(Vector3 pos, Vector3 scale, Vector3 front);
	void Update(double dt);
	
	void Move(Vector3 dir, double dt);
	void Dash(Vector3 dir, double dt);
	void Shoot(Vector3 dir);
	// Setters
	void SetMoving(bool isMoving);
	void SetDashed(bool isDashed);

	// Issers
	bool IsMoving();
	bool IsDashed();

	float cooldownTimer = 0;

private:
	bool isMoving;
	bool isDashed;
	float forceMagnitude = 0;

	Weapon *weapon;
};


#endif