#include "Detlaff.h"


CDetlaff::CDetlaff() : target(NULL)
{
}


CDetlaff::~CDetlaff()
{
	if (target)
		delete target;
}

void CDetlaff::Update(double dt)
{
	GameObject::Update(dt);

	//< THIS BOSS WILL BE AN IMMOBILE JUGGERNAUGHT
}

void CDetlaff::HandleInteraction(GameObject *b, double dt)
{
	if (CheckCollision(b, dt))
	{
		CollisionResponse(b);
	}
}

void CDetlaff::FireOnTarget(Entity* target)
{
	this->target = target;

	//player upper right and bottom left boundaries
	Vector3 upperRight = Vector3(target->GetPosition().x + 5, target->GetPosition().y + 5, 0);
	Vector3 bottomLeft = Vector3(target->GetPosition().x - 5, target->GetPosition().y - 5, 0);

	//create a vector of type Vector3 to store the locations
	std::vector<Vector3>location;
	//clear the vector
	location.clear();

	//Push in 3 locations for the bomb
	for (int i = 0; i < 3; i++)
	{
		location.push_back(Vector3(Math::RandFloatMinMax(upperRight.x, bottomLeft.x), Math::RandFloatMinMax(upperRight.x, bottomLeft.x), 0));
	}
	

}