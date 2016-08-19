#include "GolemHead.h"

GolemHead::GolemHead()
/*link(NULL)*/
{
}

void GolemHead::Init(Vector3 pos)
{
    this->pos = pos;
    active = true;
    type = GameObject::GO_ENTITY;
    team = TEAM_ENEMY;
    collider.type = Collider::COLLIDER_BOX;
    mass = 1000;
    destinationCountdown = REACH_CHECKER;
    speedLimit = 5.0f;
    movementSpeed = 1.0f;
    health = 2500.f;

    captureRatio = 1.f;
}

GolemHead::~GolemHead()
{

}

void GolemHead::Update(double dt)
{
    GameObject::Update(dt);
    if (vel.IsZero() == false)
        front = vel.Normalized();

    // No Movement on Head
}