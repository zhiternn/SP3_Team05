/******************************************************************************/
/*!
\file	GolemHead.h
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Class to handle Boss Golem Head
*/
/******************************************************************************/
#ifndef GOLEMHEAD_H
#define GOLEMHEAD_H

#include "Enemy.h"

/******************************************************************************/
/*!
Class GolemHead.h:
\brief	Class to handle Boss Golem Head
*/
/******************************************************************************/
class GolemHead : public Enemy
{
public:
    GolemHead();
    ~GolemHead();

    virtual void Update(double dt);
    virtual void HandleInteraction(GameObject* b, double dt);

private:

};

#endif /* GOLEMHEAD_H */