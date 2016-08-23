#include "GolemHand.h"
#include "GolemHead.h"
#include "MeshManager.h"
#include "Player.h"

GolemRightHand::GolemRightHand() :
Enemy()
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
	entityType = Entity::ENTITY_BOSS_BODY;
    team = TEAM_ENEMY;
    collider.type = Collider::COLLIDER_BALL;
    mass = 4;
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

void GolemRightHand::SetupMesh()
{
    float degree = Math::RadianToDegree(atan2(front.y, front.x));
    modelStack.Translate(pos.x, pos.y, pos.z);
    modelStack.Rotate(degree, 0, 0, 1);
    modelStack.Rotate(60, 0, 1, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    mesh = meshList[GEO_GOLEMHAND];
}

void GolemRightHand::HandleInteraction(GameObject* b, double dt)
{
    if (CheckCollision(b, dt))
    {
        CollisionResponse(b);

        Player* player = dynamic_cast<Player*>(b);
        if (player)
        {
            player->TakeDamage(ATTACK_COLLIDE_DAMAGE);
        }
    }
}

GolemLeftHand::GolemLeftHand() :
Enemy()
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
    mass = 4;
    destinationCountdown = 5.f;
    speedLimit = 45.f;
    movementSpeed = 800.f;

    captureRatio = 1.f;
    state = 0;
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
                state == 2;
            }

            if (state == 2)
            {
                Vector3 destination = (target->pos - 50) + offsetDir.Normalized();
                ChangeDestination(MOVETO_TARGET, destination);
                if (Reached(destination))
                {
                    destination = (target->pos + 40) + offsetDir.Normalized();
                    ChangeDestination(MOVETO_TARGET, destination);
                }
                state = 1;
            }

        }
    }
}

void GolemLeftHand::SetupMesh()
{
    float degree = Math::RadianToDegree(atan2(front.y, front.x));
    modelStack.Translate(pos.x, pos.y, pos.z);
    modelStack.Rotate(degree, 0, 0, 1);
    modelStack.Rotate(60, 0, 1, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    mesh = meshList[GEO_GOLEMHAND];
}

void GolemLeftHand::HandleInteraction(GameObject* b, double dt)
{
    if (CheckCollision(b, dt))
    {
        CollisionResponse(b);

        Player* player = dynamic_cast<Player*>(b);
        if (player)
        {
            player->TakeDamage(ATTACK_COLLIDE_DAMAGE);
        }
    }
}