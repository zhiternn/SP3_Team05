#include "ProjectileList.h"

void Rope::Update(double dt)
{
	CProjectile::Update(dt);
}

void Rope::HandleInteraction(GameObject* b, double dt)
{

}

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