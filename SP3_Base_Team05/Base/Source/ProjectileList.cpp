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

		//CalculateChance(b);
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