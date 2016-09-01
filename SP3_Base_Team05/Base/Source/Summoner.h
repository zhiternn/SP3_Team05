#ifndef SUMMONER_H
#define SUMMONER_H

#include "Enemy.h"
#include "Summons.h"

static const float SUMMONER_TOUCH_DAMAGE = 10.f;
static const float ATTACK_COOLDOWN = 1.5f;
static const float SUMMONING_COOLDOWN = 3.f;
static const float SUMMONER_HEALTH_REGEN_PERSEC = 10.f;
static int AMOUNT_OF_SUMMONS = 6;

class Summoner : public Enemy
{
public:
	Summoner();
	~Summoner();

	virtual void Init(Vector3 pos);
	virtual void Update(double dt);
	virtual void TakeDamage(unsigned amount);
	virtual void SetupMesh();
	virtual void HandleInteraction(GameObject* b, double dt);
	virtual void HandleOutOfBounds(float minX, float maxX, float minY, float maxY);

	void CleaningUpMess();
	void Defend();
	void Attack();
	void UpdateCooldowns(double dt);

private:
	float safetyThreshold;
	float chaseThreshold;
	float agressiveLevel;
	float summonCooldownTimer = 0;
	float attackCooldownTimer = 3;
	bool attacking;
	bool teleported;

	std::vector<Summons*> summonsList;

};

Summons* FetchSummons();

#endif // !SUMMONER_H
