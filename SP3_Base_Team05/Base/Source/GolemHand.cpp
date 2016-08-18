#include "GolemHand.h"

GolemHand::GolemHand() :
Enemy()/*,
link(NULL)*/
{
}

GolemHand::~GolemHand()
{
    if (target)
        delete target;
}

void GolemHand::Update(double dt)
{
    GameObject::Update(dt);

    if (!Enemy::UpdateMovement(dt))
    {
        if (target)
        {
            float offset = Math::RandFloatMinMax(5, 15);
            Vector3 offsetDir = target->pos - pos;
            offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);

            Vector3 destination = target->pos + offsetDir.Normalized() * offset;
            AddDestination(destination);
        }
        else
        {
            Vector3 temp(Math::RandFloatMinMax(200, 250), Math::RandFloatMinMax(200, 250), 0);
            AddDestination(temp);
        }
    }
}

void GolemHand::SetTarget(Entity* target)
{

}