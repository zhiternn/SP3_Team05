#ifndef GRENADE_H
#define GRENADE_H

#include "Projectile.h"

class Grenade : public CProjectile
{
public:
	Grenade();
	~Grenade();

	virtual void Update(double dt);

private:

};

#endif // !GRENADE_H
