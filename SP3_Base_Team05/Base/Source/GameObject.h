#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Collider.h"

#include <vector>

class GameObject
{
public:
	enum GAMEOBJECT_TYPE
	{
		GO_BALL = 0,
		GO_CUBE,

		GO_END, //must be last
	};

	static std::vector<GameObject*> goList;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	virtual ~GameObject();

	virtual void Update(double dt);

	GAMEOBJECT_TYPE type;
	Collider collider;

	Vector3 pos;
	Vector3 vel;
	Vector3 front;
	Vector3 scale;

	bool active;

	float mass;

	void ApplyForce(double dt, Vector3 direction, float magnitude = 0.0f);
	float GetForce();

	virtual bool CheckCollision(GameObject* b, double dt);
	virtual float CheckCollision2(GameObject* b);
	virtual void CollisionResponse(GameObject* b);
};

GameObject* FetchGO();

#endif