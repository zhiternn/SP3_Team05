#include "Detlaff.h"
#include "WeaponList.h"
#include "ProjectileList.h"


CDetlaff::CDetlaff() : target(NULL), state(STATE_1)
{
	//set weapon to STATE_1 default weapon
	weapon = new SplitGun();
}


CDetlaff::~CDetlaff()
{
	if (target)
		delete target;

	if (weapon)
		delete weapon;
}

void CDetlaff::Update(double dt)
{
	GameObject::Update(dt);

	//< THIS BOSS WILL BE AN IMMOBILE JUGGERNAUT

	//Update States
	if (this->GetHP() <= 0)
	{
		delete weapon;
		switch (state)
		{
			case STATE_1:
			{
				//Spawn Stage 2 Boss
				state = STATE_2;
				this->health = 200;
				//weapon = new MachineGun();
				break;
			}
			case STATE_2:
			{
				//Spawn State 3 Boss
				state = STATE_3;
				this->health = 400;
				//weapon = new BIGGRENADABOMBHEHEXD()
				break;
			}
		}
	}
}

void CDetlaff::HandleInteraction(GameObject *b, double dt)
{
	if (CheckCollision(b, dt))
	{
		CollisionResponse(b);
	}
}
