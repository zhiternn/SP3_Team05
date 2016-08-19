#ifndef SUMMONER_H
#define SUMMONER_H

#include "Enemy.h"
#include "Summons.h"

class Summoner : public Enemy
{
public:
	Summoner();
	~Summoner();

	virtual void Init(Vector3 pos);
	virtual void Update(double dt);
	virtual void TakeDamage(unsigned amount);

	void CleaningUpMess();
	void Defend();
	void Attack();

private:
	float safetyThreshold;
	float chaseThreshold;
	float agressiveLevel;
	float maxHealth; 
	std::vector<Summons*> summonsList;

};


#endif // !SUMMONER_H
