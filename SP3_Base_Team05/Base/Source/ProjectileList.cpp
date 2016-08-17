#include "ProjectileList.h"

/*
ROPE
*/
void Rope::Update(double dt)
{
	CProjectile::Update(dt);
}

void Rope::HandleInteraction(GameObject* b, double dt)
{

}


/*
SHIELD
*/

void Shield::Update(double dt)
{
	CProjectile::Update(dt);
}
void Shield::HandleInteraction(GameObject* b, double dt)
{
	if (b->GetType() == GameObject::GO_PROJECTILE)
	{
		if (CheckCollision(b, dt))
			b->SetActive(false);
	}
}

/*
TRAP
*/
void Trap::HandleInteraction(GameObject *b, double dt)
{
	//If GameObject type is of GO_ENEMY
	if (b->GetType() == GameObject::GO_ENTITY)
	{
	}
}

void Trap::CalculateChance(unsigned int enemyHealth)
{
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