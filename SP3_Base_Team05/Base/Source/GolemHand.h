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

    void LinkTo(Entity* entity);

private:
    Entity* link;
    float stopdelay;
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

    void LinkTo(Entity* entity);

private:
    Entity* link;
};

#endif /* GOLEMHAND_H */