#ifndef BOSS_H
#define BOSS_H

#include <stack>

#include "Vector3.h"
#include "GameObject.h"

class Boss : public GameObject
{
public:
	Boss();
	~Boss();

	void Goto(Vector3 pos);
	void AddDestination(Vector3 pos);

private:
	Vector3 pos;
	std::stack<Vector3> destination;

};


#endif // !Boss
