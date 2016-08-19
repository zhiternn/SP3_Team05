#include "Detlaff.h"
#include "WeaponList.h"
#include "ProjectileList.h"


CDetlaff::CDetlaff() : target(NULL), state(STATE_1)
{
	//set weapon to STATE_1 default weapon
	weapon = new SplitGun(360.f, 72);
	weapon->AssignProjectile(new Bullet());
	this->SetRate(0.f);
}


CDetlaff::~CDetlaff()
{
	if (target)
		delete target;
}

void CDetlaff::Update(double dt)
{
	GameObject::Update(dt);

	//< THIS BOSS WILL BE AN IMMOBILE JUGGERNAUT

	//Update States
	if (this->GetHP() <= 0)
	{
		switch (state)
		{
			case STATE_1:
			{
				//Spawn Stage 2 Boss
				state = STATE_2;
				this->health = 200;
				//Create the stage 2 splitgun
				weapon = new SplitGun(45.f, 10);
				weapon->AssignProjectile(new Bullet());
				break;
			}
			case STATE_2:
			{
				//Spawn State 3 Boss
				state = STATE_3;
				this->health = 400;
				this->SetRate(0.35f);
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
