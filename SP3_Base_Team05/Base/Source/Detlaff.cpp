#include "Detlaff.h"
#include "WeaponList.h"
#include "ProjectileList.h"


CDetlaff::CDetlaff() : target(NULL), state(STATE_1)
{
	//set weapon to STATE_1 default weapon
	weapon = new Splitgun(360.f, 36);
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
	weapon->Update(dt);
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
				weapon = new Splitgun(45.0f, 6);
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

void CDetlaff::Shoot(Vector3 dir)
{
	this->weapon->Fire(this->pos, dir, CProjectile::TEAM_ENEMY);
}