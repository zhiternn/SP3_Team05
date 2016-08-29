/******************************************************************************/
/*!
\file	TutorialMob.cpp
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Class to define Tutorial Monster
*/
/******************************************************************************/

#include "TutorialMob.h"
#include "MeshManager.h"
#include "Player.h"

/******************************************************************************/
/*!
\brief	TutorialMob Default Constructor
*/
/******************************************************************************/
TutorialMob::TutorialMob()
{

}

/******************************************************************************/
/*!
\brief	TutorialMob Destructor
*/
/******************************************************************************/
TutorialMob::~TutorialMob()
{

}


void TutorialMob::Init(Vector3 pos)
{
    this->pos = pos;
    active = true;
    type = GameObject::GO_ENTITY;
    entityType = Entity::ENTITY_BOSS_MAIN;
    team = TEAM_ENEMY;
    collider.type = Collider::COLLIDER_BALL;
    mass = 5;
    destinationCountdown = REACH_CHECKER;
    speedLimit = 50.f;
    movementSpeed = 300.f;
    health = 100.f; 
	maxHealth = health;
    isDead = false;
    regendelay = 0;
    mobGun = new MachineGun();
    mobGun->AssignProjectile(new Hook());
    mobGun->SetFireRate(0.1f);
}

/******************************************************************************/
/*!
\brief
Update the Tutorial Mob and Projectile

\param dt
Time active
*/
/******************************************************************************/
void TutorialMob::Update(double dt)
{
    GameObject::Update(dt);
    mobGun->Update(dt);

    if (vel.IsZero() == false)
        front = vel.Normalized();

    if (!Enemy::UpdateMovement(dt))
    {
        if (target)
        {
            float combinedRadius = scale.x + target->GetScale().x;
            Vector3 offsetDir = (target->pos - pos);
            offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);
            float offset = combinedRadius;

            Vector3 destination = (target->pos) + offsetDir.Normalized();
            ChangeDestination(MOVETO_TARGET, destination);

            if (Reached(destination))
            {
                destination = (target->pos * 1.5) + offsetDir.Normalized();
                ChangeDestination(MOVETO_TARGET, destination);
                destinationCountdown = 5.0f;
            }
        }

    }

    //tutorialMobShoot(target->GetFront());
}
void TutorialMob::tutorialMobShoot(Vector3 dir)
{
    Vector3 playerPos = (dir - this->pos).Normalized();
    mobGun->Fire(this->pos, playerPos, team);
}

void TutorialMob::SetupMesh()
{
    modelStack.Translate(pos.x, pos.y, pos.z);
    modelStack.Rotate(60, 0, 1, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    mesh = meshList[GEO_GOLEMHEAD];
}

void TutorialMob::HandleInteraction(GameObject* b, double dt)
{
    if (CheckCollision(b, dt))
    {
        CollisionResponse(b);

        Player* player = dynamic_cast<Player*>(b);
        if (player)
        {
            player->TakeDamage(20);
        }
    }
}