/******************************************************************************/
/*!
\file	TutorialMob.h
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Class to define Tutorial Monster
*/
/******************************************************************************/
#ifndef TUTORIALMOB_H
#define TUTORIALMOB_H

#include "Enemy.h"
#include "WeaponList.h"

static const int TUTORIAL_MOB_DMG = 5;

/******************************************************************************/
/*!
Class TutorialMob.h:
\brief	Class to define Tutorial Monster
*/
/******************************************************************************/
class TutorialMob : public Enemy
{
public:
    TutorialMob();                                              //< Tutorial Monster Default Constructor
    ~TutorialMob();                                             //< Tutorial Monster Destructor

    virtual void Init(Vector3 pos);                             //< Initialize the Position and other values of Tutorial Mob
    virtual void Update(double dt);                             //< Update the Tutorial Mob (position and things)
    virtual void SetupMesh();                                   //< Sets up mesh for the Monster
    virtual void HandleInteraction(GameObject* b, double dt);   //< Handles Interaction with Player Taking damage from monster

    void tutorialMobShoot(Vector3 dir);

private:
    float safetyThreshold;
    float chaseThreshold;
    float agressiveLevel;
    float regendelay;
    Weapon *mobGun;
};

#endif // !SUMMONER_H
