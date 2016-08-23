#include "ProjectileList.h"
#include "Rope.h"
#include "ForceField.h"

void Hook::Update(double dt)
{
	CProjectile::Update(dt);
}

void Hook::HandleInteraction(GameObject* b, double dt)
{
	if (this->team == b->GetTeam())
		return;

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
				rope->Init(this->pos, b, this->ropeLifetime);
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
	if (this->team == b->GetTeam())
		return;

	if (CheckCollision(b, dt))
	{
		CollisionResponse(b);
		if (b->GetType() == GameObject::GO_ENTITY)
		{
			Entity* entity = dynamic_cast<Entity*>(b);
			if (entity)
			{
				entity->TakeDamage(proj_dmg);

				ForceField *field;
				field->Init(this->pos, b, this->fieldDuration);
				field->Update(dt);
			}
		}

		this->SetActive(false);
	}
}


bool Trap::Capture()
{
	return false;
}

void Trap::Update(double dt)
{
	CProjectile::Update(dt);
}


void Shield::Init(Vector3 pos)
{
    this->pos = pos;
    this->active = true;
    type = GameObject::GO_PROJECTILE;
    team = TEAM_PLAYER;
    //SetDMG(0);
    //SetLifetime(999);
    //SetProjectileSpeed(0);
    //SetScale(4, 4, 4);
    //SetCurrHealth(2000);
    //SetMaxHealth(2000);
    //SetAliveState(false);

    proj_dmg = 0;
    aliveState = true;
    CurrHealth = 2000;
    MaxHealth = 2000;
    startShieldRegen = false;
    proj_speed = 0;
    proj_lifetime = 10;
    collider.type = Collider::COLLIDER_BOX;
}


void Shield::Update(double dt)
{
    GameObject::Update(dt);
}

void Shield::HandleInteraction(GameObject* b, double dt)
{
	if (this->team == b->GetTeam())
		return;

    if (CheckCollision(b, dt))
    {
        //Entity* entity = dynamic_cast<Entity*>(b);
        //if (entity)
        //{
        //    entity->HandleInteraction(b, dt);
        //    //CollisionResponse(b);
        //}

        CollisionResponse(b);
        if (b->GetType() == GameObject::GO_ENTITY)
        {
            Entity* entity = dynamic_cast<Entity*>(b);
            if (entity)
            {
                entity->HandleInteraction(b, dt);
            }
        }
    }
}

void Shield::regenerateShield(float currHP, double dt)
{
    if (active == false && CurrHealth < MaxHealth)
    {
        CurrHealth += dt * 2;
    }
}

void Shield::TakeDamage(unsigned amount)
{
    CurrHealth -= amount;
    if (CurrHealth <= 0)
    {
        aliveState = false;
        active = false;
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
	if (this->team == b->GetTeam())
		return;

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