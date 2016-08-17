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