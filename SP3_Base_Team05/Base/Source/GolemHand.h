/******************************************************************************/
/*!
\file	GolemHand.h
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Class to handle Boss Golem Head
*/
/******************************************************************************/
#ifndef GOLEMHAND_H
#define GOLEMHAND_H

#include "Enemy.h"

/******************************************************************************/
/*!
Class GolemHand.h:
\brief	Class to handle Boss Golem Head
*/
/******************************************************************************/
class GolemHand : public Enemy
{
public:
    GolemHand();
    ~GolemHand();

    virtual void Update(double dt);

    void SetTarget(Entity* target);

private:
    Entity* target;
};

#endif /* GOLEMHAND_H */