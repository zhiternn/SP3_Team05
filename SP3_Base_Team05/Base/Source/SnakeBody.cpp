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
	team = TEAM_ENEMY;
	entityType = Entity::ENTITY_BOSS_BODY;
	movementSpeed = speed;
	this->speedLimit = speedLimit;
	collider.type = Collider::COLLIDER_BALL;
	health = 300;
	maxHealth = health;
	this->scale.Set(7, 7, 7);
}

void SnakeBody::Update(double dt)
{
	if (this->vel.LengthSquared() > speedLimit * speedLimit)
		this->vel = this->vel.Normalized() * speedLimit;

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
	if (body)//skips checks against other bodies
		return;

	if (b->GetType() == GameObject::GO_ENTITY)
	{
		if (this->team != b->GetTeam())
		{
			if (CheckCollision(b, dt))
			{
				Player* player = dynamic_cast<Player*>(b);
				if (player)
					player->TakeDamage(ATTACK_RAM_DAMAGE);
				else
				{
					Enemy* enemy = static_cast<Enemy*>(b);
					enemy->TakeDamage(ATTACK_RAM_DAMAGE);
				}
				CollisionResponse(b);

				return;
			}
		}
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