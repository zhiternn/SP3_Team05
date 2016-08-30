#include "Trap.h"
#include "MeshManager.h"
#include "Enemy.h"

Trap::Trap() :
GameObject(GO_ENVIRONMENT),
radius(20.f)
{
}

Trap::~Trap()
{
}

void Trap::Init(Vector3 pos, float size)
{
	type = GameObject::GO_ENVIRONMENT;
	this->pos = pos;
	this->mass = 0;
	this->vel.SetZero();
	lifetime = 1.0f;
	this->radius = size;
	scale.Set(radius, radius, radius);
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
		float halfSpeed = enemy->GetSpeedLimit() * 0.5f;
		if (enemy->GetVelocity().LengthSquared() > halfSpeed * halfSpeed)
			enemy->SetVelocity(enemy->GetVelocity().Normalized() * halfSpeed);

		enemy->SetCapturing(true);
		this->lifetime = 1.0f;
	}
}

void Trap::SetupMesh()
{
	static float degree = 0.0f;
	degree += 0.1f;

	if (degree > 360.0f)
		degree -= 360.0f;

	modelStack.Translate(pos.x, pos.y, pos.z + 0.01f);
	modelStack.Rotate(degree, 0, 0, 1);
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