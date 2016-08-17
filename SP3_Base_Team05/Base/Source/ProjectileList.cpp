#include "ProjectileList.h"

void Rope::HandleInteraction(GameObject* b, double dt)
{

}

void Shield::HandleInteraction(GameObject* b, double dt)
{
	if (b->GetType() == GameObject::GO_PROJECTILE)
	{
		if (CheckCollision(b, dt))
			b->SetActive(false);
	}
}