#include "Rope.h"
Rope::Rope():
GameObject(GO_ENVIRONMENT),
maxLength(20.0f),
target(NULL)
{
}

Rope::~Rope()
{
	if (target)
		delete target;
}

void Rope::Init(Vector3 pos, GameObject* target)
{
	this->pos = pos;
	this->target = target;
	collider.type = Collider::COLLIDER_NONE;
}

void Rope::Update(double dt)
{
	if (!target)
		return;

	if (target->pos != pos)
	{
		front = target->pos - pos;
		this->scale.x = (target->pos - pos).Length();

		if (!CheckCollision(target, dt))
		{
			Vector3 N = (target->pos - this->pos).Normalized();
			float difference = scale.x - maxLength;
			
			target->SetVelocity(target->GetVelocity() - N * difference);
		}
	}
}

void Rope::HandleInteraction(double dt)
{
	return;
}

Rope* FetchRope()
{
	std::vector<GameObject*>::iterator it;
	for (it = GameObject::goList.begin(); it != GameObject::goList.end(); ++it)
	{
		Rope *rope = dynamic_cast<Rope*>((*it));
		if (rope && rope->IsActive() == false)
		{
			rope->GameObject::SetType(GameObject::GO_ENTITY);
			rope->SetActive(true);
			return rope;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		GameObject::goList.push_back(new Rope());
	}
	Rope *rope = dynamic_cast<Rope*>(*(GameObject::goList.end() - 10));
	if (rope)
	{
		rope->GameObject::SetType(GameObject::GO_ENTITY);
		rope->SetActive(true);
		return rope;
	}

   { //for safety measure
	   Rope *rope = new Rope();
	   rope->GameObject::SetType(GameObject::GO_ENTITY);
	   rope->SetActive(true);
	   GameObject::goList.push_back(rope);
	   return rope;
   }
}