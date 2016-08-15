#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Init(Vector3 pos, Vector3 scale, Vector3 front);
	void UpdateInputs();
	void Update(double dt);

private:

};


#endif