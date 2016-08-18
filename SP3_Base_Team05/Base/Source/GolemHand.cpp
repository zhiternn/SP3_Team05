#include "GolemHand.h"

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
    this->pos = pos;
    active = true;
    type = GameObject::GO_ENTITY;
    team = TEAM_ENEMY;
    collider.type = Collider::COLLIDER_BALL;
    mass = 1;
    destinationCountdown = 5.f;
    speedLimit = 50.f;
    movementSpeed = 10.f;

    captureRatio = 1.f;
}

void GolemRightHand::Update(double dt)
{
    GameObject::Update(dt);

    if (!Enemy::UpdateMovement(dt))
    {
        if (target)
        {
            //float combinedRadius = scale.x + target->GetScale().x;
            //float offset = Math::RandFloatMinMax(combinedRadius * 2, combinedRadius * 3);
            //Vector3 offsetDir = target->pos - pos;
            //offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);

            //Vector3 destination = target->pos + offsetDir.Normalized() * offset;
            //ChangeDestination(MOVETO_TARGET, destination);

            float combinedRadius = scale.x + target->GetScale().x;
            Vector3 offsetDir = (target->pos - pos);
            offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);
            float offset = combinedRadius * 2;

            Vector3 destination = (target->pos * 0.5) + offsetDir.Normalized();
            ChangeDestination(MOVETO_TARGET, destination);
            if (Reached(destination))
            {
                destination = (target->pos * 2) + offsetDir.Normalized();
                ChangeDestination(MOVETO_TARGET, destination);
            }


            {//testy stuff
                static GameObject* hehe = NULL;
                GameObject* go = FetchGO();
                go->SetActive(true);

                go->pos = destination;
                if (hehe)
                    hehe->SetActive(false);
                hehe = go;
            }
        }

    }

    //if (!Enemy::UpdateMovement(dt))
    //{
    //    if (link)
    //    {
    //        if (pos != link->pos)
    //        {
    //            Vector3 toLink = (link->pos - pos).Normalized();
    //        }
    //    }
    //}

    //if (vel.IsZero() == false)
    //    front = vel.Normalized();

    //if (!Enemy::UpdateMovement(dt))
    //{
    //    if (target)
    //    {
    //        float combinedRadius = scale.x + target->GetScale().x;
    //        float offset = Math::RandFloatMinMax(combinedRadius * 1.5f, combinedRadius * 2.0f);
    //        Vector3 offsetDir = target->pos - pos;
    //        offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);

    //        Vector3 destination = target->pos + offsetDir.Normalized() * offset;
    //        ChangeDestination(MOVETO_TARGET, destination);
    //    }
    //}

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
    this->pos = pos;
    active = true;
    type = GameObject::GO_ENTITY;
    team = TEAM_ENEMY;
    collider.type = Collider::COLLIDER_BALL;
    mass = 1;
    destinationCountdown = REACH_CHECKER;
    speedLimit = 50.f;
    movementSpeed = .0f;

    captureRatio = 1.f;
}

void GolemLeftHand::Update(double dt)
{
    GameObject::Update(dt);
    if (!Enemy::UpdateMovement(dt))
    {
        if (link)
        {
            if (pos != link->pos)
            {
                Vector3 toLink = (link->pos - pos).Normalized();
            }
        }
    }

    if (vel.IsZero() == false)
        front = vel.Normalized();

    if (!Enemy::UpdateMovement(dt))
    {
        if (target)
        {
            float combinedRadius = scale.x + target->GetScale().x;
            float offset = Math::RandFloatMinMax(combinedRadius * 5.f, combinedRadius * 5.f);
            Vector3 offsetDir = target->pos - pos;
            offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);

            Vector3 destination = target->pos + offsetDir.Normalized() * offset;
            ChangeDestination(MOVETO_TARGET, destination);
        }
    }
}

void GolemLeftHand::LinkTo(Entity* entity)
{
    link = entity;
}