#include "Detlaff.h"
#include "WeaponList.h"
#include "ProjectileList.h"
#include "MeshManager.h"


CDetlaff::CDetlaff() : target(NULL), state(STATE_1)
{
	entityType = Entity::ENTITY_BOSS_MAIN;
	//set weapon to STATE_1 default weapon
	weapon = new Splitgun(360.f, 36);
	weapon->AssignProjectile(new Bullet());
	weapon_extra = NULL;
	this->SetHP(1000);
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
	if (this->weapon_extra != NULL)
	{
		weapon_extra->Update(dt);
	}
	//< THIS BOSS WILL BE AN IMMOBILE JUGGERNAUT

	//Update States
	if (this->GetHP() <= 200)
	{
		switch (state)
		{
			case STATE_1:
			{
				//Spawn Stage 2 Boss
				state = STATE_2;
				this->health = 1500;
				//Create the stage 2 splitgun
				weapon = new Splitgun(45.0f, 12);
				weapon->AssignProjectile(new Bullet());
				break;
			}
			case STATE_2:
			{
				//Spawn State 3 Boss
				state = STATE_3;
				this->health = 3000;
				weapon = new Splitgun(90.f, 12);
				weapon->AssignProjectile(new Hook());
				weapon->GetProjInfo()->SetProjectileSpeed(100.0f);
				weapon_extra = new Shotgun();
				weapon_extra->AssignProjectile(new Bullet());
				weapon_extra->GetProjInfo()->SetScale(2.f, 2.f, 2.f);
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
	if (weapon_extra != NULL)
	{
		this->weapon_extra->Fire(this->pos, dir, CProjectile::TEAM_ENEMY);
	}
}

void CDetlaff::Teleport(float m_worldWidth, float m_worldHeight)
{
	float randPos_X, randPos_Y;

	randPos_X = Math::RandFloatMinMax(0, m_worldWidth);
	randPos_Y = Math::RandFloatMinMax(0, m_worldHeight);

	this->pos.Set(randPos_X, randPos_Y, 0);
}

void CDetlaff::SetupMesh()
{
	Vector3 right = front.Cross(Vector3(0, 0, 1));

	modelStack.Translate(pos.x, pos.y, pos.z + 1);
	//modelStack.Rotate(-pitchDegree, right.x, right.y, right.z);
	modelStack.Scale(scale.x, scale.y, scale.z);

	mesh = meshList[GEO_DETLAFF_1];
}