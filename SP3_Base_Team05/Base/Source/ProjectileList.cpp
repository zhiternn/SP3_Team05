#include "ProjectileList.h"
#include "Rope.h"

void Hook::Update(double dt)
{
	CProjectile::Update(dt);
}

void Hook::HandleInteraction(GameObject* b, double dt)
{
	if (CheckCollision(b, dt))
	{
		CollisionResponse(b);
		if (b->GetType() == GameObject::GO_ENTITY)
		{
			Entity* entity = dynamic_cast<Entity*>(b);
			if (entity)
			{
				entity->TakeDamage(proj_dmg);

				Rope* rope = FetchRope();
				rope->Init(this->pos, b);
			}
		}

		this->SetActive(false);
	}
}

/*
TRAP
*/
void Trap::HandleInteraction(GameObject *b, double dt)
{
}

//void Trap::CalculateChance(Enemy *enemy)
//{
//	captureChance = ((100 - enemy->GetHP()) * enemy->GetRate()) / 100;
//}

bool Trap::Capture()
{
	float lucky7 = Math::RandFloatMinMax(0, 1.f);

	if (lucky7 > this->captureChance)
	{
		//unlucky 7 top kek
		return false;
	}
	else
	{
		//actually lucky 7 massive kek
		return true;
	}
}

void Trap::Update(double dt)
{
	CProjectile::Update(dt);
}


void Shield::Update(double dt)
{
    CProjectile::Update(dt);
    if (activeState == false)
        SetLifetime(0);

    if (CurrHealth <= 0)
    {
        SetLifetime(0);
    }

    regenerateShield(CurrHealth, dt);
}

void Shield::HandleInteraction(GameObject* b, double dt)
{
    //If GameObject type is of GO_ENEMY
    if (b->GetType() == GameObject::GO_ENTITY)
    {
        ////Prevents them from moving
        //b->SetVelocity(0, 0, 0);

        ////Remove the trap once enemy is constrained
        //this->SetActive(false);

        //CalculateChance(b);
        CurrHealth -= 10;
        if (CurrHealth <= 0)
        {
            SetLifetime(0);
            SetActiveState(false);
        }
    }

}

void Shield::regenerateShield(float currHP, double dt)
{
    if (activeState == false && CurrHealth < MaxHealth)
    {
        CurrHealth += dt * 2;
    }
}

/******************************************************************************/
/*!
\brief
Get Current Health to get the Current Health

\return
Current Shield Health
*/
/******************************************************************************/
float Shield::GetCurrHealth()
{
    return this->CurrHealth;
}

/******************************************************************************/
/*!
\brief0.

Get Max Health to get the Max Health

\return
Max Shield Health
*/
/******************************************************************************/
float Shield::GetMaxHealth()
{
    return MaxHealth;
}

/******************************************************************************/
/*!
\brief
Returns Projectile Shield Active State

\return
Projectile Shield Active State
*/
/******************************************************************************/
bool Shield::GetActiveState()
{
    return activeState;
}

/******************************************************************************/
/*!
\brief
Returns Projectile Shield Alive State

\return
Projectile Shield Alive State
*/
/******************************************************************************/
bool Shield::GetAliveState()
{
    return aliveState;
}

/******************************************************************************/
/*!
\brief
Setting the Projectile Shield Active State

\param active
The new active to replace the old one
*/
/******************************************************************************/
void Shield::SetActiveState(bool active)
{
    activeState = active;
}

/******************************************************************************/
/*!
\brief
Setting the Projectile Shield Alive State

\param alive
The new alive to replace the old one
*/
/******************************************************************************/
void Shield::SetAliveState(bool alive)
{
    aliveState = alive;
}

/******************************************************************************/
/*!
\brief
Setting the Projectile Shield Max HP

\param maxhp
The new maxhp to replace the old one
*/
/******************************************************************************/
void Shield::SetMaxHealth(float max)
{
    MaxHealth = max;
}

/******************************************************************************/
/*!
\brief
Setting the Projectile Shield Curr HP

\param currhp
The new currhp to replace the old one
*/
/******************************************************************************/
void Shield::SetCurrHealth(float curr)
{
    CurrHealth = curr;
}

void Bullet::Update(double dt)
{
	CProjectile::Update(dt);
}

void Bullet::HandleInteraction(GameObject *b, double dt)
{
	if (CheckCollision(b, dt))
	{
		CollisionResponse(b);
		if (b->GetType() == GameObject::GO_ENTITY)
		{
			Entity* entity = dynamic_cast<Entity*>(b);
			if (entity)
			{
				entity->TakeDamage(proj_dmg);
			}
		}

		this->SetActive(false);
	}
}

Bullet* FetchBullet()
{
	std::vector<GameObject*>::iterator it;
	for (it = GameObject::goList.begin(); it != GameObject::goList.end(); ++it)
	{
		Bullet *proj = dynamic_cast<Bullet*>((*it));
		if (proj && proj->IsActive() == false)
		{
			proj->GameObject::SetType(GameObject::GO_PROJECTILE);
			proj->SetActive(true);
			return proj;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		GameObject::goList.push_back(new Bullet());
	}
	Bullet *proj = dynamic_cast<Bullet*>(*(GameObject::goList.end() - 10));
	if (proj)
	{
		proj->GameObject::SetType(GameObject::GO_PROJECTILE);
		proj->SetActive(true);
		return proj;
	}

	{ //for safety measure
		Bullet *proj = new Bullet();
		proj->GameObject::SetType(GameObject::GO_PROJECTILE);
		proj->SetActive(true);
		GameObject::goList.push_back(proj);
		return proj;
	}
}

Hook* FetchHook()
{
	std::vector<GameObject*>::iterator it;
	for (it = GameObject::goList.begin(); it != GameObject::goList.end(); ++it)
	{
		Hook *proj = dynamic_cast<Hook*>((*it));
		if (proj && proj->IsActive() == false)
		{
			proj->GameObject::SetType(GameObject::GO_PROJECTILE);
			proj->SetActive(true);
			return proj;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		GameObject::goList.push_back(new Hook());
	}
	Hook *proj = dynamic_cast<Hook*>(*(GameObject::goList.end() - 10));
	if (proj)
	{
		proj->GameObject::SetType(GameObject::GO_PROJECTILE);
		proj->SetActive(true);
		return proj;
	}

	{ //for safety measure
		Hook *proj = new Hook();
		proj->GameObject::SetType(GameObject::GO_PROJECTILE);
		proj->SetActive(true);
		GameObject::goList.push_back(proj);
		return proj;
	}
}