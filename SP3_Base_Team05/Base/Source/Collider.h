#ifndef COLLIDER_H
#define COLLIDER_H

struct Collider
{
	enum COLLIDER_TYPE
	{
		COLLIDER_NONE,
		COLLIDER_BALL,
		COLLIDER_BOX,

		COLLIDER_END, //must be last
	};

	COLLIDER_TYPE type;
	bool isStatic;
	bool isTrigger;
};

#endif