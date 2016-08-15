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

	void ApplyForce(double dt, Vector3 direction, float magnitude = 0.0f);
	float GetForce();

	void HandleInteraction(GameObject* b, double dt);

	// Getters
	Vector3 GetPosition();
	Vector3 GetVelocity();
	Vector3 GetFront();
	Vector3 GetScale();
	GAMEOBJECT_TYPE GetType();
	Collider GetCollider();
	bool GetActive();
	float GetMass();

	// Setters
	void SetPostion(float x, float y, float z);
	void SetPostion(Vector3 pos);
	void SetVelocity(float x, float y, float z);
	void SetVelocity(Vector3 vel);
	void SetScale(float x, float y, float z);
	void SetScale(Vector3 scale);
	void SetFront(float x, float y, float z);
	void SetFront(Vector3 front);
	void SetType(GAMEOBJECT_TYPE type);
	void SetColliderType(Collider::COLLIDER_TYPE type);
	void SetMass(float mass);
	void SetActive(bool active);

protected:
	virtual bool CheckCollision(GameObject* b, double dt);
	virtual float CheckCollision2(GameObject* b);
	virtual void CollisionResponse(GameObject* b);

	Vector3 pos;
	Vector3 vel;
	Vector3 front;
	Vector3 scale;
	bool active;

	GAMEOBJECT_TYPE type;
	Collider collider;

	float mass;

};

GameObject* FetchGO();

#endif