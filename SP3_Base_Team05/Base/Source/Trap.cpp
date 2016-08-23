#include "Trap.h"
#include "MeshManager.h"

Trap::Trap() :
GameObject(GO_ENVIRONMENT),
radius(20.0f),
target(NULL),
lifetime(5.0f)
{
}

Trap::~Trap()
{
	if (target)
		delete target;
}

void Trap::Init(Vector3 pos, GameObject* target, float lifeTime)
{
	type = GameObject::GO_ENVIRONMENT;
	this->pos = pos;
	this->target = target;
	lifetime = lifeTime;
	scale.Set(radius, radius, radius);
	collider.type = Collider::COLLIDER_NONE;
}

void Trap::Update(double dt)
{
	if (!target)
		return;
	lifetime -= dt;
	if (lifetime <= 0)
		active = false;

	std::cout << lifetime << std::endl;
}

void Trap::HandleInteraction(double dt)
{
	return;
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
			trap->GameObject::SetType(GameObject::GO_ENTITY);
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
		trap->GameObject::SetType(GameObject::GO_ENTITY);
		trap->SetActive(true);
		return trap;
	}

   { //for safety measure
	   Trap *trap = new Trap();
	   trap->GameObject::SetType(GameObject::GO_ENTITY);
	   trap->SetActive(true);
	   GameObject::goList.push_back(trap);
	   return trap;
   }
}