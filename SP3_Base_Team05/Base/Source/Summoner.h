#ifndef SUMMONER_H
#define SUMMONER_H

#include "Enemy.h"

class Summoner : public Enemy
{
public:
	Summoner();
	~Summoner();

	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);

private:

};


#endif // !SUMMONER_H
