#ifndef SUMMONS_H
#define SUMMONS_H

#include "Enemy.h"

class Summons : public Enemy
{
public:
	Summons();
	~Summons();

	virtual void Init(Vector3 pos);
	virtual void Update(double dt);
	void Goto(Vector3 pos);

private:
	float innerThreshold;
	float outerThreshold;

};


#endif // !SUMMONS_H
