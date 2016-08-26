#ifndef SUMMONER_H
#define SUMMONER_H

#include "Enemy.h"
#include "Summons.h"

static const float ATTACK_COOLDOWN = 1.5f;
static const float SUMMONING_COOLDOWN = 3.f;
static const float HEALTH_REGEN_PERSEC = 10.f;
static int AMOUNT_OF_SUMMONS = 0;

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

	void CleaningUpMess();
	void Defend();
	void Attack();
	void UpdateCooldowns(double dt);

private:
	float safetyThreshold;
	float chaseThreshold;
	float agressiveLevel;
	float maxHealth;
	float summonCooldownTimer = 0;
	float attackCooldownTimer = 3;
	bool attacking;

	std::vector<Summons*> summonsList;

};

Summons* FetchSummons();

#endif // !SUMMONER_H
