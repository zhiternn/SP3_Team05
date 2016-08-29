#include "GolemHead.h"
#include "MeshManager.h"
#include "Player.h"

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
    collider.type = Collider::COLLIDER_BALL;
    mass = 999;
    destinationCountdown = REACH_CHECKER;
    speedLimit = 10.f;
    movementSpeed = 150.f;
    health = 4000.f;
    maxhealth = 4000.f;
    isDead = false;
    regendelay = 0;
    golemGun = new Splitgun(360.f, 18);
    golemGun->AssignProjectile(new Bullet());
    golemGun->SetFireRate(0.1f);
    scale.Set(12, 12, 12);
}

GolemHead::~GolemHead()
{

}

void GolemHead::Update(double dt)
{
    GameObject::Update(dt);
    golemGun->Update(dt);
    if (vel.IsZero() == false)
        front = vel.Normalized();

    regendelay += 1;

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
                destination = (target->pos * 1.5) + offsetDir.Normalized();
                ChangeDestination(MOVETO_TARGET, destination);
                destinationCountdown = 5.0f;
            }
        }

    }
    if (health < (maxhealth) * 0.75)
        golemGun->SetFireRate(0.2f);
    if (health < (maxhealth)* 0.50)
        golemGun->SetFireRate(0.35f);
    if (health < (maxhealth) * 0.1)
        golemGun->SetFireRate(0.4f);

    if (regendelay >= 10)
    {
        if (health < (maxhealth * 0.25))
            health += 15;
        else if (health < (maxhealth * 0.5))
            health += 12;
        else if (health < maxhealth)
            health += 7;

        if (health >= maxhealth)
            health = maxhealth;

        regendelay = 0;
    }
    //std::cout << health << std::endl;
    // No Movement on Head
    GolemShoot(target->GetFront());
}

void GolemHead::SetupMesh()
{
    modelStack.Translate(pos.x, pos.y, pos.z);
    modelStack.Rotate(60, 0, 1, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    mesh = meshList[GEO_GOLEMHEAD];
}

void GolemHead::GolemShoot(Vector3 dir)
{
    Vector3 playerPos = (dir - this->pos).Normalized();
    golemGun->Fire(this->pos, playerPos, team);
}

void GolemHead::HandleInteraction(GameObject* b, double dt)
{
    if (CheckCollision(b, dt))
    {
        CollisionResponse(b);

		Entity* entity = dynamic_cast<Entity*>(b);
		if (entity && this->team != entity->GetTeam())
		{
			entity->TakeDamage(20);
		}
    }
}