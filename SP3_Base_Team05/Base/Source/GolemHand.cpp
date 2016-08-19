#include "GolemHand.h"
#include "GolemHead.h"

GolemRightHand::GolemRightHand() :
Enemy(),
link(NULL)
{
}

GolemRightHand::~GolemRightHand()
{
}

void GolemRightHand::Init(Vector3 pos)
{
    Enemy::Init(pos);
    this->pos = pos;
    active = true;
    type = GameObject::GO_ENTITY;
    team = TEAM_ENEMY;
    collider.type = Collider::COLLIDER_BALL;
    mass = 100;
    destinationCountdown = 5.f;
    speedLimit = 45.f;
    movementSpeed = 800.f;
    health = 999999;

    captureRatio = 1.f;
    stopdelay = 0.1f;
}

void GolemRightHand::Update(double dt)
{
    GameObject::Update(dt);

    if (!Enemy::UpdateMovement(dt))
    {
        if (target)
        {
            float combinedRadius = scale.x + target->GetScale().x;
            Vector3 offsetDir = (target->pos - pos);
            offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);
            float offset = combinedRadius * 1.5;

            Vector3 destination = (target->pos * 0.7) + offsetDir.Normalized();
            ChangeDestination(MOVETO_TARGET, destination);

            if (Reached(destination))
            {
                stopdelay -= dt;
                if (stopdelay <= 0)
                {
                    destination = (target->pos * 1.5) + offsetDir.Normalized();
                    ChangeDestination(MOVETO_TARGET, destination);
                    destinationCountdown = 5.0f;
                    stopdelay = 0.1f;
                }
            }
        }

    }
}

void GolemRightHand::LinkTo(Entity* entity)
{
    link = entity;
}

GolemLeftHand::GolemLeftHand() :
Enemy(),
link(NULL)
{
}

GolemLeftHand::~GolemLeftHand()
{
}

void GolemLeftHand::Init(Vector3 pos)
{
    Enemy::Init(pos);
    this->pos = pos;
    health = 999999;
    active = true;
    type = GameObject::GO_ENTITY;
    team = TEAM_ENEMY;
    collider.type = Collider::COLLIDER_BALL;
    mass = 100;
    destinationCountdown = 5.f;
    speedLimit = 45.f;
    movementSpeed = 800.f;

    captureRatio = 1.f;
}

void GolemLeftHand::Update(double dt)
{
    GameObject::Update(dt);
    if (!Enemy::UpdateMovement(dt))
    {
        if (target)
        {
            float combinedRadius = scale.x + target->GetScale().x;
            Vector3 offsetDir = (target->pos - pos);
            offsetDir.Set(offsetDir.y, -offsetDir.x, offsetDir.z);
            float offset = combinedRadius * 2;

            Vector3 destination = (target->pos + 50) + offsetDir.Normalized();
            ChangeDestination(MOVETO_TARGET, destination);
            if (Reached(destination))
            {
                destination = (target->pos - 40) + offsetDir.Normalized();
                ChangeDestination(MOVETO_TARGET, destination);
            }

        }
    }
}

void GolemLeftHand::LinkTo(Entity* entity)
{
    link = entity;
}