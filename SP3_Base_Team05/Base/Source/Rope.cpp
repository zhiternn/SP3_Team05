#include "Rope.h"
#include "MeshManager.h"

Rope::Rope():
GameObject(GO_ENVIRONMENT),
maxLength(20.0f),
target(NULL),
lifetime(5.0f)
{
}

Rope::~Rope()
{
	if (target)
		delete target;
}

void Rope::Init(Vector3 pos, GameObject* target, float lifeTime, float length)
{
	type = GameObject::GO_ENVIRONMENT;
	this->pos = pos;
	this->vel.SetZero();
	this->target = target;
	lifetime = lifeTime;
	collider.type = Collider::COLLIDER_NONE;
	maxLength = length;
}

void Rope::Update(double dt)
{
	if (!target)
	{
		active = false;
		return;
	}
	lifetime -= dt;
	
	if (lifetime <= 0 || target->IsActive() == false)
	{
		target = NULL;
		active = false;
		return;
	}
	if (target->pos != pos)
	{
		front = target->pos - pos;
		this->scale.x = (target->pos - pos).Length();

		if (this->scale.x > maxLength)
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

void Rope::SetupMesh()
{
	float yawDegree = Math::RadianToDegree(atan2(front.y, front.x));
	
	modelStack.Translate(pos.x, pos.y, pos.z + 0.01f);
	modelStack.Rotate(yawDegree, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	modelStack.Translate(0.5f, 0, 0);
	mesh = meshList[GEO_ROPE];
}

Rope* FetchRope()
{
	std::vector<GameObject*>::iterator it;
	for (it = GameObject::goList.begin(); it != GameObject::goList.end(); ++it)
	{
		Rope *rope = dynamic_cast<Rope*>((*it));
		if (rope && rope->IsActive() == false)
		{
			rope->GameObject::SetType(GameObject::GO_ENVIRONMENT);
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
		rope->GameObject::SetType(GameObject::GO_ENVIRONMENT);
		rope->SetActive(true);
		return rope;
	}

   { //for safety measure
	   Rope *rope = new Rope();
	   rope->GameObject::SetType(GameObject::GO_ENVIRONMENT);
	   rope->SetActive(true);
	   GameObject::goList.push_back(rope);
	   return rope;
   }
}