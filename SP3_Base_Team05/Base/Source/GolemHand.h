/******************************************************************************/
/*!
\file	GolemHand.h
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Class to handle Boss Golem Hand
*/
/******************************************************************************/
#ifndef GOLEMHAND_H
#define GOLEMHAND_H

#include "Enemy.h"
#include "WeaponList.h"
#include "Player.h"

static const int ATTACK_COLLIDE_DAMAGE = 5;

/******************************************************************************/
/*!
Class GolemHand.h:
\brief	Class to handle Boss Golem Hand
*/
/******************************************************************************/
class GolemRightHand : public Enemy
{
public:
    GolemRightHand();
    ~GolemRightHand();

    virtual void Init(Vector3 pos);
    virtual void Update(double dt);

    virtual void SetupMesh();
    virtual void HandleInteraction(GameObject* b, double dt);

private:
    float stopdelay;
    float speedboost, speedtime;
    float handSpeed, handLimit;

};        

/******************************************************************************/
/*!
Class GolemHand.h:
\brief	Class to handle Boss Golem Hand
*/
/******************************************************************************/
class GolemLeftHand : public Enemy
{
public:
    GolemLeftHand();
    ~GolemLeftHand();

    virtual void Init(Vector3 pos);
    virtual void Update(double dt);

    virtual void SetupMesh();
    virtual void HandleInteraction(GameObject* b, double dt);
    virtual void TakeDamage(unsigned amount);

    void Shoot();

private:
    float state, firegogo, pewpew;                                          //< Firing State and Rate of Hand
    Weapon* golemLHGun;                                                     //< Golem's Left Hand
};

#endif /* GOLEMHAND_H */