#include "Summoner.h"

Summoner::Summoner()
{
}

Summoner::~Summoner()
{
}

void Summoner::Update(double dt)
{
	Enemy::Update(dt);
	Enemy::UpdateMovement(dt);
}

void Summoner::HandleInteraction(GameObject* b, double dt)
{

}