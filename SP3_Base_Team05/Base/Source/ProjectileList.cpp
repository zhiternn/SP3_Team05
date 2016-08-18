#include "ProjectileList.h"

void ShotgunShell::Update(double dt)
{
    CProjectile::Update(dt);
}

void ShotgunShell::HandleInteraction(GameObject* b, double dt)
{
    if (b->GetType() == GameObject::GO_PROJECTILE)
    {
        if (CheckCollision(b, dt))
            b->SetActive(false);
    }
}


void Rope::Update(double dt)
{
	CProjectile::Update(dt);
}

void Rope::HandleInteraction(GameObject* b, double dt)
{
}

/*
TRAP
*/
void Trap::HandleInteraction(GameObject *b, double dt)
{
	//If GameObject type is of GO_ENEMY
	if (b->GetType() == GameObject::GO_ENTITY)
	{
		//Prevents them from moving
		b->SetVelocity(0, 0, 0);
		//Remove the trap once enemy is constrained
		this->SetActive(false);

		CalculateChance(dynamic_cast<Enemy*>(b));
		if (Capture())
		{
			b->SetActive(false);
			//Return a Higher Score to the player
			// or maybe money
		}

	}
}

void Trap::CalculateChance(Enemy *enemy)
{
	captureChance = ((100 - enemy->GetHP()) * enemy->GetRate()) / 100;
}

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
	if (b->GetType() == GameObject::GO_PROJECTILE)
	{
		if (CheckCollision(b, dt))
			b->SetActive(false);
	}
}