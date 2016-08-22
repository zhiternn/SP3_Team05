#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Inventory.h"

static const float MOVEMENT_LIMIT = 300.f;
static const float DASH_DISTANCE = 50.f;
static const float DASH_COOLDOWN = 1.f;

class Player : public Entity
{
public:
	Player();
	~Player();


	// Functions
	void Init(Vector3 pos, Vector3 scale, Vector3 front);
	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);
	virtual void SetupMesh();
	
	void Move(Vector3 dir, double dt);
	void Dash(Vector3 dir, double dt);
	void Shoot(Vector3 dir);
    void Shield(Vector3 dir);
	void ChangeWeapon(int type);
	void ChangeWeaponUp();
	void ChangeWeaponDown();

	// Setters
	void SetMoving(bool isMoving);
	void SetDashed(bool isDashed);

	// Issers 
	bool IsMoving();
	bool IsDashed();
	Inventory *inventory;
	Weapon *weapon;
	float cooldownTimer = 0;
	short weaponIter = 0;

private:
	bool isMoving;
	bool isDashed;
	float forceMagnitude = 0;
};


#endif