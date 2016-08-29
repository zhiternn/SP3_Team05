#include "SnakeBody.h"
#include "ProjectileList.h"
#include "MeshManager.h"
#include "SnakeHead.h"
#include "Player.h"

SnakeBody::SnakeBody() :
Enemy(),
weapon(NULL),
pitchDegree(0.0f)
{
	if (weapon)
	{
		if (weapon->GetProjInfo())
			delete weapon->GetProjInfo();
		delete weapon;
	}

	weapon = new Splitgun(120, 3, 0.5f);
	weapon->AssignProjectile(new Bullet(10, 8, 80, 2, 1));
}

SnakeBody::~SnakeBody()
{
	if (weapon)
		delete weapon;
}

void SnakeBody::Init(Vector3 pos, float speed, float speedLimit)
{
	Enemy::Init(pos);
	team = TEAM_PLAYER;
	entityType = Entity::ENTITY_BOSS_BODY;
	movementSpeed = speed;
	this->speedLimit = speedLimit;
	collider.type = Collider::COLLIDER_BALL;
	health = 300;
	this->scale.Set(5, 5, 5);
}

void SnakeBody::Update(double dt)
{
	GameObject::Update(dt);

	weapon->Update(dt);

	if (this->vel.IsZero() == false)
		this->front = vel.Normalized();

	//for animation
	pitchDegree += vel.LengthSquared() * dt;//Math::Wrap(pitchDegree + vel.LengthSquared() * (float)dt, 0.0f, 360.0f);
}

void SnakeBody::HandleInteraction(GameObject* b, double dt)
{
	SnakeHead* head = dynamic_cast<SnakeHead*>(b);
	if (head)//skips checks against head
		return;
	SnakeBody* body = dynamic_cast<SnakeBody*>(b);
	if (body)//skips checks against body
		return;

	Entity* entity = dynamic_cast<Entity*>(b);
	if (entity && this->team != entity->GetTeam())
	{
		if (CheckCollision(b, dt))
			entity->TakeDamage(DAMAGE_ONTOUCH);
	}

	GameObject::HandleInteraction(b, dt);
}

void SnakeBody::Die()
{
	this->isDead = true;
}

void SnakeBody::GoTo(Vector3 destination, double dt)
{
	if (Reached(destination))
		return;

	this->vel += (destination - this->pos).Normalized() * this->movementSpeed * dt;
	if (this->vel.LengthSquared() > speedLimit * speedLimit)
		this->vel = this->vel.Normalized() * speedLimit;
}

void SnakeBody::Shoot(Vector3 target)
{
	if (weapon)
		weapon->Fire(this->pos, target - this->pos, team);
}

void SnakeBody::SetupMesh()
{
	Vector3 right = front.Cross(Vector3(0, 0, 1));

	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(-pitchDegree, right.x, right.y, right.z);
	modelStack.Scale(scale.x, scale.y, scale.z);

	if (isDead)
		meshList[GEO_SNAKE_BODY]->material.kAmbient.Set(0.3f, 0.0f, 0.0f);
	else
		meshList[GEO_SNAKE_BODY]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);

	mesh = meshList[GEO_SNAKE_BODY];
}


//#include "SnakeBody.h"
//#include "ProjectileList.h"
//#include "MeshManager.h"
//#include "SnakeHead.h"
//
//SnakeBody::SnakeBody():
//pitchDegree(0.0f),
//Enemy(),
//backLink(NULL),
//weapon(NULL)
//{
//}
//
//SnakeBody::~SnakeBody()
//{
//	if (backLink)
//		delete backLink;
//	if (weapon)
//		delete weapon;
//}
//
//void SnakeBody::Init(Vector3 pos, float speed, float speedLimit)
//{
//	Enemy::Init(pos);
//	entityType = Entity::ENTITY_BOSS_BODY;
//	movementSpeed = speed;
//	this->speedLimit = speedLimit;
//	collider.type = Collider::COLLIDER_BALL;
//	health = 50;
//
//	if (weapon)
//	{
//		if (weapon->GetProjInfo())
//			delete weapon->GetProjInfo();
//		delete weapon;
//	}
//
//	weapon = new Splitgun(70, 3, 1);
//	weapon->AssignProjectile(new Bullet(10, 8, 80, 2, 1));
//}
//
//void SnakeBody::Update(double dt)
//{
//	GameObject::Update(dt);
//
//	if (vel.IsZero() == false)
//		front = vel.Normalized();
//
//	if (!isDead)
//	{
//		weapon->Update(dt);
//
//		if (vel.LengthSquared() > speedLimit * speedLimit)
//		{
//			vel = vel.Normalized() * speedLimit;
//		}
//
//		if (backLink)
//		{
//			if (!backLink->IsDead())
//				Pull(backLink);
//			else
//				Reconnect();
//		}
//	}
//	else
//	{
//		if (target && !target->IsDead())
//		{
//			if (target->pos != pos)
//			{
//				vel += (target->pos - pos).Normalized() * movementSpeed * 0.1f;
//
//				float halfLimit = speedLimit * 0.5f;
//				if (vel.LengthSquared() > halfLimit * halfLimit)
//					vel = vel.Normalized() * halfLimit;
//			}
//
//		}
//	}
//
//	//for animation
//	pitchDegree += vel.LengthSquared() * dt;//Math::Wrap(pitchDegree + vel.LengthSquared() * (float)dt, 0.0f, 360.0f);
//}
//
//void SnakeBody::HandleInteraction(GameObject* b, double dt)
//{
//	if (CheckCollision(b, dt))
//	{
//		SnakeHead* head = dynamic_cast<SnakeHead*>(b);
//		if (head)
//		{
//			head->HandleInteraction(this, dt);
//		}
//		else
//		{
//			CollisionResponse(b);
//		}
//	}
//}
//
//void SnakeBody::Die()
//{
//	isDead = true;
//	vel.SetZero();
//}
//
//void SnakeBody::Pull(SnakeBody* body)
//{
//	Vector3 point = pos - front * scale.x;
//	if (!body->Reached(point))
//	{
//		body->Goto(point);
//	}
//}
//
//void SnakeBody::LinkBackTo(SnakeBody* entity)
//{
//	backLink = entity;
//}
//
//void SnakeBody::Goto(Vector3 pos)
//{
//	vel += (pos - this->pos).Normalized() * movementSpeed;
//}
//
//void SnakeBody::Reconnect()
//{
//	if (backLink->GetBackLink())
//	{
//		SnakeBody* link = backLink->GetBackLink();
//		if (link)
//			backLink->LinkBackTo(link);
//		else
//			backLink->LinkBackTo(NULL);
//	}
//}
//
//void SnakeBody::Fire(Entity* target)
//{
//	if (weapon)
//		weapon->Fire(this->pos, target->pos - this->pos, team);
//
//	if (backLink)
//		backLink->Fire(target);
//}
//
//SnakeBody* SnakeBody::GetBackLink()
//{
//	return backLink;
//}
//
//void SnakeBody::SetupMesh()
//{
//	Vector3 right = front.Cross(Vector3(0, 0, 1));
//
//	modelStack.Translate(pos.x, pos.y, pos.z);
//	modelStack.Rotate(-pitchDegree, right.x, right.y, right.z);
//	modelStack.Scale(scale.x, scale.y, scale.z);
//
//	if (isDead)
//		meshList[GEO_SNAKE_BODY]->material.kAmbient.Set(0.3f, 0.0f, 0.0f);
//	else
//		meshList[GEO_SNAKE_BODY]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
//
//	mesh = meshList[GEO_SNAKE_BODY];
//}
