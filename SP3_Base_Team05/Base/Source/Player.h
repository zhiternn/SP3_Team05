#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Inventory.h"

static const float MOVEMENT_LIMIT = 300.f;
static const float DASH_DISTANCE = 50.f;
static const float DASH_COOLDOWN = 1.f;
static const float DAMAGE_BUFFER = 0.5f;
static const float PLAYER_HEALTH_REGEN_COOLDOWN = 5.0f;
static const float PLAYER_HEALTH_REGEN = 1.0f;

class Player : public Entity
{
public:
	Player();
	~Player();

	// Functions
	void Init(Vector3 pos);
	virtual void Update(double dt);
	virtual void SetupMesh();
	virtual void TakeDamage(int amount);
	virtual void HandleInteraction(GameObject* b, double dt);
	
	void Move(Vector3 dir, double dt);
	void Dash(Vector3 dir, double dt);
	void Shoot(Vector3 dir);
    void Shielding(Vector3 dir);
	void ChangeWeapon(int type);
	void ChangeWeaponUp();
	void ChangeWeaponDown();
	void ChangeProjectileUp();
	void ChangeProjectileDown();

	// Setters
	void SetMoving(bool isMoving);
	void SetDashed(bool isDashed);

	// Issers
	bool IsMoving();
	bool IsDashed();

	Inventory *inventory;
	Weapon *weapon;
	CProjectile *projectile;
	float cooldownTimer = 0;
	short weaponIter = 0;
    Shield *shield;
	short projectileIter = 0;
	float maxHealth;
	bool shooting;

private:
	float damageBuffer;
	bool isMoving;
	bool isDashed;
	float forceMagnitude = 0;
	int currency;
};


#endif