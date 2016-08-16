#ifndef GUN_H
#define GUN_H

#include "Weapon.h"

class Gun : public Weapon
{
public:
	Gun();
	~Gun();

private:
	WEAPON_TYPE type;

};


#endif // !GUN_H
