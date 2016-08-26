#ifndef SNAKEBODY_H
#define SNAKEBODY_H

#include "Enemy.h"
#include "WeaponList.h"

const static float CHARGE_FORCE = 100.0f;
const static int DAMAGE_ONTOUCH = 5;

class SnakeBody : public Enemy
{
public:
	SnakeBody();
	~SnakeBody();

	virtual void Init(Vector3 pos, float speed, float speedLimit);
	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);
	virtual void SetupMesh();
	virtual void Die();

	void GoTo(Vector3 destination, double dt);
	void Shoot(Vector3 target);

private:
	Weapon* weapon;

	//for animation
	float pitchDegree;
};

#endif

//#ifndef SNAKEBODY_H
//#define SNAKEBODY_H
//
//#include "Enemy.h"
//#include "WeaponList.h"
//
//class SnakeBody : public Enemy
//{
//public:
//	SnakeBody();
//	~SnakeBody();
//
//	virtual void Init(Vector3 pos, float speed, float speedLimit);
//	virtual void Update(double dt);
//	virtual void HandleInteraction(GameObject* b, double dt);
//	virtual void SetupMesh();
//	virtual void Die();
//
//	void LinkBackTo(SnakeBody* entity);
//	
//	void Fire(Entity* target);
//
//	void Goto(Vector3 pos);
//
//	SnakeBody* GetBackLink();
//
//private:
//	void Reconnect();
//	void Pull(SnakeBody* body);
//
//	SnakeBody* backLink;
//	Weapon* weapon;
//
//	//for animation
//	float pitchDegree;
//};
//
//#endif