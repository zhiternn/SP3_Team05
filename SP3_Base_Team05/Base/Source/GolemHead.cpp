#include "GolemHead.h"
#include "MeshManager.h"

GolemHead::GolemHead()
/*link(NULL)*/
{
}

void GolemHead::Init(Vector3 pos)
{
    this->pos = pos;
    active = true;
    type = GameObject::GO_ENTITY;
	entityType = Entity::ENTITY_BOSS_MAIN;
    team = TEAM_ENEMY;
    collider.type = Collider::COLLIDER_BOX;
    mass = 10000;
    destinationCountdown = REACH_CHECKER;
    speedLimit = 0.f;
    movementSpeed = 0.f;
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

void GolemHead::SetupMesh()
{
    float degree = Math::RadianToDegree(atan2(front.y, front.x));
    modelStack.Translate(pos.x, pos.y, pos.z);
    modelStack.Rotate(degree, 0, 0, 1);
    modelStack.Scale(scale.x, scale.y, scale.z);
    mesh = meshList[GEO_CUBE];
}