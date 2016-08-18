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
	virtual void HandleInteraction(GameObject* b, double dt);

	void UpdateSummons(double dt);
	void Defend();

private:
	float safetyThreshold;
	float chaseThreshold;
	std::vector<Summons*> summonsList;

};


#endif // !SUMMONER_H
