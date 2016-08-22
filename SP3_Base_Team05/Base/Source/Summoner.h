#ifndef SUMMONER_H
#define SUMMONER_H

#include "Enemy.h"
#include "Summons.h"

static const float SUMMONING_COOLDOWN = 3.f;
static int AMOUNT_OF_SUMMONS = 7;

class Summoner : public Enemy
{
public:
	Summoner();
	~Summoner();

	virtual void Init(Vector3 pos);
	virtual void Update(double dt);
	virtual void TakeDamage(unsigned amount);
	virtual void SetupMesh();

	void CleaningUpMess();
	void Defend();
	void Attack();

private:
	float safetyThreshold;
	float chaseThreshold;
	float agressiveLevel;
	float maxHealth;
	float cooldownTimer = 0;
	std::vector<Summons*> summonsList;

};


#endif // !SUMMONER_H
