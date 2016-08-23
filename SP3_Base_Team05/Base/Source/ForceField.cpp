#include "ForceField.h"
#include "MeshManager.h"

ForceField::ForceField() :
GameObject(GO_ENVIRONMENT),
maxRadius(20.0f),
target(NULL),
lifetime(5.0f)
{
}

ForceField::~ForceField()
{
	if (target)
		delete target;
}

void ForceField::Init(Vector3 pos, GameObject* target, float lifeTime)
{
	type = GameObject::GO_ENVIRONMENT;
	this->pos = pos;
	this->target = target;
	lifetime = lifeTime;
	collider.type = Collider::COLLIDER_NONE;
}

void ForceField::Update(double dt)
{
	if (!target)
		return;
	lifetime -= dt;

	if (lifetime <= 0)
		active = false;
	if (target->pos != pos)
	{
		front = target->pos - pos;
		this->scale.x = (target->pos - pos).Length();

		if (!CheckCollision(target, dt))
		{
			Vector3 N = (target->pos - this->pos).Normalized();
			float difference = scale.x - maxRadius;

			target->SetVelocity(target->GetVelocity() - N * difference);
		}
	}
}

void ForceField::HandleInteraction(double dt)
{
	return;
}

void ForceField::SetupMesh()
{
	float yawDegree = Math::RadianToDegree(atan2(front.y, front.x));

	modelStack.Translate(pos.x, pos.y, pos.z + 0.01f);
	modelStack.Rotate(yawDegree, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	modelStack.Translate(0.5f, 0, 0);
	mesh = meshList[GEO_FIELD];
}

ForceField* FetchForceField()
{
	std::vector<GameObject*>::iterator it;
	for (it = GameObject::goList.begin(); it != GameObject::goList.end(); ++it)
	{
		ForceField *forceField = dynamic_cast<ForceField*>((*it));
		if (forceField && forceField->IsActive() == false)
		{
			forceField->GameObject::SetType(GameObject::GO_ENTITY);
			forceField->SetActive(true);
			return forceField;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		GameObject::goList.push_back(new ForceField());
	}

	ForceField *forceField = dynamic_cast<ForceField*>(*(GameObject::goList.end() - 10));
	if (forceField)
	{
		forceField->GameObject::SetType(GameObject::GO_ENTITY);
		forceField->SetActive(true);
		return forceField;
	}

   { //for safety measure
	   ForceField *forceField = new ForceField();
	   forceField->GameObject::SetType(GameObject::GO_ENTITY);
	   forceField->SetActive(true);
	   GameObject::goList.push_back(forceField);
	   return forceField;
   }
}