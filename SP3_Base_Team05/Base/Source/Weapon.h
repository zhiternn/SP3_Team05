/******************************************************************************/
/*!
\file	Weapon.h
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Generic Class for Weapons
*/
/******************************************************************************/
#ifndef WEAPON_H
#define WEAPON_H

/******************************************************************************/
/*!
Class GameObject.h:
\brief	Creates Weapon for the game
*/
/******************************************************************************/
enum WEAPON_TYPE
{
	GUN,
	ROPEGUN,
	TRAP,
	SHIELD,
	GRENADE,
	WEAPON_TOTAL
};

class Weapon
{
public:
	Weapon();		//< Default Constructor
	~Weapon();		//< Destructor


};

#endif /* WEAPON_H */