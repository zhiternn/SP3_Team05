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
    mass = 2;
    destinationCountdown = 5.f;
    speedLimit = 45.f;
    movementSpeed = 800.f;
    health = 999999;
    scale.Set(8, 8, 8);

    stopdelay = 0.1f;
    isGrabPlayer = false;
    targetPlayer = 0.f;
}

void GolemRightHand::Update(double dt)
{
    GameObject::Update(dt);
    if (vel.IsZero() == false)
        front = vel.Normalized();

    health = 999999;

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

void GolemRightHand::GrabPlayer(GameObject* b, double dt)
{
    //GolemHead* head = dynamic_cast<GolemHead*>(b);
    //for (int i = 0; i < GameObject::goList.size(); ++i)
    //{
    //    if (GameObject::goList[i]->IsActive())
    //    {
    //        //GameObject::goList[i]->
    //        Entity* entity = dynamic_cast<Entity*>(GameObject::goList[i]);
    //    }
    //}
    //ChangeDestination(MOVETO_TARGET, head->GetPosition());
    //destinationCountdown = 20.f;

    //targetPlayer += (float)dt;

    //if (targetPlayer >= 5)
    //{
    //    speedLimit = 545.f;
    //    movementSpeed = 3800.f;
    //    if (CheckCollision(b, dt))
    //    {
    //        Player* player = dynamic_cast<Player*>(b);

    //        ChangeDestination(MOVETO_TARGET, head->pos);
    //        Vector3 tempPos(pos.x + front.x * (scale.x + 4), pos.y + front.y * (scale.y + 4), 0);
    //        player->pos = tempPos;

    //        if (Reached(head->pos))
    //        {
    //            destinationCountdown = 0.f;
    //            targetPlayer = 0;
    //        }
    //    }
    //    if (targetPlayer >= 10)
    //    {
    //        targetPlayer = 0;
    //        destinationCountdown = 0.f;
    //        speedLimit = 45.f;
    //        movementSpeed = 800.f;
    //    }
    //}
    //else
    //{
    //    
    //}
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

void GolemRightHand::Action(float ratio)
{
    //float rand = Math::RandFloatMinMax(0, 1);
    //if (rand <= ratio)//attack
    //{
    //    if (Math::RandInt() % 2)
    //        //GrabPlayer();
    //    else
    //    {

    //    }
    //}

}

void GolemRightHand::HandleInteraction(GameObject* b, double dt)
{
    if (CheckCollision(b, dt))
    {
        CollisionResponse(b);

		Entity* entity = dynamic_cast<Entity*>(b);
		if (entity && this->team != entity->GetTeam())
        {
			entity->TakeDamage(ATTACK_COLLIDE_DAMAGE);
        }
    }
    //float rand = Math::RandFloatMinMax(0, 1);
    //if (rand <= 0.7)
    //{
    //    GrabPlayer(b, dt);
    //}
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
}

void GolemLeftHand::Update(double dt)
{
    GameObject::Update(dt);
    if (vel.IsZero() == false)
        front = vel.Normalized();

    health = 999999;

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

void GolemLeftHand::HandleInteraction(GameObject* b, double dt)
{
    if (CheckCollision(b, dt))
    {
        CollisionResponse(b);

		Entity* entity = dynamic_cast<Entity*>(b);
		if (entity && (this->team != entity->GetTeam()))
        {
			entity->TakeDamage(ATTACK_COLLIDE_DAMAGE);
        }
    }
}