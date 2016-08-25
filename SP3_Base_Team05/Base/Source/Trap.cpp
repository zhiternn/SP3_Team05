#include "Trap.h"
#include "MeshManager.h"
#include "Enemy.h"

Trap::Trap() :
GameObject(GO_ENVIRONMENT),
radius(20.0f)
{
}

Trap::~Trap()
{
}

void Trap::Init(Vector3 pos)
{
	type = GameObject::GO_ENVIRONMENT;
	this->pos = pos;
	this->mass = 0;
	this->vel.SetZero();
	lifetime = 1.0f;
	scale.Set(radius, radius, 2);
	collider.type = Collider::COLLIDER_BALL;
	collider.isTrigger = true;
}

void Trap::Update(double dt)
{
	GameObject::Update(dt);
	lifetime -= dt;

	if (lifetime <= 0)
		active = false;
}

void Trap::HandleInteraction(GameObject* b, double dt)
{
	Enemy *enemy = dynamic_cast<Enemy*>(b);
	if (!enemy)
		return;

	float combinedRadius = this->scale.x + b->GetScale().x;
	float distanceBetween = (this->pos - b->pos + b->GetVelocity() * (float)dt).LengthSquared();
	if (distanceBetween <= combinedRadius * combinedRadius)
	{
		enemy->Capturing(dt);
		this->lifetime = 1.0f;
	}
}

void Trap::SetupMesh()
{
	modelStack.Translate(pos.x, pos.y, pos.z + 0.01f);
	modelStack.Scale(scale.x, scale.y, scale.z);
	mesh = meshList[GEO_TRAP];
}

Trap* FetchTrap()
{
	std::vector<GameObject*>::iterator it;
	for (it = GameObject::goList.begin(); it != GameObject::goList.end(); ++it)
	{
		Trap *trap = dynamic_cast<Trap*>((*it));
		if (trap && trap->IsActive() == false)
		{
			trap->GameObject::SetType(GameObject::GO_ENVIRONMENT);
			trap->SetActive(true);
			return trap;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		GameObject::goList.push_back(new Trap());
	}
	Trap *trap = dynamic_cast<Trap*>(*(GameObject::goList.end() - 10));
	if (trap)
	{
		trap->GameObject::SetType(GameObject::GO_ENVIRONMENT);
		trap->SetActive(true);
		return trap;
	}

   { //for safety measure
	   Trap *trap = new Trap();
	   trap->GameObject::SetType(GameObject::GO_ENVIRONMENT);
	   trap->SetActive(true);
	   GameObject::goList.push_back(trap);
	   return trap;
   }
}