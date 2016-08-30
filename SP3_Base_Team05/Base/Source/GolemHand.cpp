#include "GolemHand.h"
#include "GolemHead.h"
#include "MeshManager.h"
#include "Player.h"
#include "Application.h"

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
    mass = 2;
    destinationCountdown = 5.f;
    speedLimit = 45.f;
    movementSpeed = 800.f;
    health = 999999;
    scale.Set(8, 8, 8);

    stopdelay = 0.1f;
    speedboost = speedtime = 0.f;
    handSpeed = 0.f;
    handLimit = 0.f;

}

void GolemRightHand::Update(double dt)
{
    GameObject::Update(dt);
    if (vel.IsZero() == false)
        front = vel.Normalized();

    health = 999999;

    speedboost += dt;

    if (speedboost >= Math::RandFloatMinMax(3.0, 7.0))
    {
        movementSpeed += 100;
        speedLimit += 2;

        speedtime += dt;
        if (speedtime >= 1.1)
        {
            speedLimit = 45.f;
            movementSpeed = 800.f;
            speedtime = 0;
            speedboost = 0;
        }
    }

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
    modelStack.Rotate(270, 0, 0, 1);
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
    entityType = Entity::ENTITY_BOSS_BODY;
    mass = 2;
    destinationCountdown = 7.f;
    speedLimit = 45.f;
    movementSpeed = 800.f;
    scale.Set(8, 8, 8);

    state = 0;
    golemLHGun = new MachineGun();
    golemLHGun->AssignProjectile(new Bullet());
    golemLHGun->SetFireRate(10.f);

    firegogo = pewpew = 0.f;
}

void GolemLeftHand::Update(double dt)
{
    GameObject::Update(dt);
    golemLHGun->Update(dt);
    if (vel.IsZero() == false)
        front = vel.Normalized();

    health = 999999;

    firegogo += dt;

    if (firegogo >= 4)
    {
        Shoot();
        pewpew += dt;
        if (pewpew >= 0.3)
        {
            firegogo = 0;
            pewpew = 0;
        }
    }
    if (!Enemy::UpdateMovement(dt))
    {
        if (target)
        {
            float combinedRadius = scale.x + target->GetScale().x;
            Vector3 offsetDir = (target->pos - pos);
            offsetDir.Set(offsetDir.y, -offsetDir.x, offsetDir.z);
            float offset = combinedRadius * 2;

            Vector3 destination;
            destination = (target->pos + 50) + offsetDir.Normalized();
            if (target->pos.x > 300)
            {
                destination = (target->pos - 50) + offsetDir.Normalized();
                state = 1;
            }
            ChangeDestination(MOVETO_TARGET, destination);
            if (Reached(destination))
            {
                if (state = 1)
                {
                    destination = (target->pos + 5) + offsetDir.Normalized();
                    ChangeDestination(MOVETO_TARGET, destination);
                    state = 0;
                }
                else
                {
                    destination = (target->pos - 60) + offsetDir.Normalized();
                    ChangeDestination(MOVETO_TARGET, destination);
                }
            }

        }
    }
}

void GolemLeftHand::Shoot()
{
    front = target->pos - this->pos;
    golemLHGun->Fire(this->pos, target->pos - this->pos, team);
}

void GolemLeftHand::SetupMesh()
{
    float degree = Math::RadianToDegree(atan2(front.y, front.x));
    modelStack.Translate(pos.x, pos.y, pos.z);
    modelStack.Rotate(270, 0, 0, 1);
    modelStack.Rotate(degree, 0, 0, 1);
    modelStack.Rotate(60, 0, 1, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    mesh = meshList[GEO_GOLEMHAND];
}
void GolemLeftHand::TakeDamage(unsigned amount)
{

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