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

    void Action(float ratio);
    void GrabPlayer(GameObject* b, double dt);

private:
    float stopdelay;
    bool isGrabPlayer, GrabbedPlayer;
    float targetPlayer;
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

private:
    float state;
};

#endif /* GOLEMHAND_H */