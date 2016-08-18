#include "GameObject.h"

std::vector<GameObject*> GameObject::goList;

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) : 
type(typeValue),
scale(1, 1, 1),
pos(0, 0, 0),
vel(0, 0, 0),
active(false),
mass(1.0f),
front(1, 0, 0)
{
	team = TEAM_NEUTRAL;
	collider.type = Collider::COLLIDER_NONE;
	collider.isStatic = false;
	collider.isTrigger = false;
}

GameObject::~GameObject()
{
}

void GameObject::Update(double dt)
{
	pos += vel * (float)dt;
}

void GameObject::ApplyForce(double dt, Vector3 direction, float magnitude)
{
	if (magnitude != 0)
	{
		direction = direction.Normalized() * magnitude;
	}
	vel += (direction / mass) * dt;
}

float GameObject::GetForce()
{
	return mass * vel.Length();
}

GameObject* FetchGO()
{
	for (std::vector<GameObject*>::iterator it = GameObject::goList.begin(); it != GameObject::goList.end(); ++it)
	{
		if ((*it)->IsActive() == false)
		{
			(*it)->SetActive(true);
			return (*it);
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		GameObject::goList.push_back(new GameObject(GameObject::GO_ENVIRONMENT));
	}

	(*(GameObject::goList.end() - 10))->SetActive(true);
	return (*(GameObject::goList.end() - 10));
}

bool GameObject::CheckCollision(GameObject* b, double dt)
{
	switch (b->collider.type)
	{
	case Collider::COLLIDER_BALL:
	{//BALL TO BALL
		float combinedRadius = this->scale.x + b->scale.x;

		return (
			//distance check
			((this->pos + this->vel * (float)dt) - b->pos).LengthSquared() <= combinedRadius * combinedRadius
			&&
			//direction check. checks relative vel against relative displacement
			(this->vel - b->vel).Dot(this->pos - b->pos) < 0
			);
	}
		break;
	case Collider::COLLIDER_BOX:
	{//BALL TO BOX
		Vector3 N = b->front;
		Vector3 NP = Vector3(N.y, -N.x, N.z);
		Vector3 goPos = this->pos + this->vel * dt;
		Vector3 go2Pos = b->pos;

		return abs((go2Pos - goPos).Dot(N)) < this->scale.x + b->scale.x * 0.5f
		   &&
		   abs((b->pos - goPos).Dot(NP)) < this->scale.x + b->scale.y * 0.5f;
	}
		break;

	default:
		return false;
		break;
	}
}

float GameObject::CheckCollision2(GameObject* b)
{
	switch (b->collider.type)
	{
	case Collider::COLLIDER_BALL:
	{
		Vector3 relVel = this->vel - b->vel;
		Vector3 relDir = this->pos - b->pos;

		//direction check. checks relative vel against relative displacement
		if (relVel.Dot(relDir) > 0)
		{
		  return -1.0f;
		}

		float combinedRadius = this->scale.x + b->scale.x;

		float A = relVel.Dot(relVel);
		float B = 2 * relVel.Dot(relDir);
		float C = relDir.Dot(relDir) - (combinedRadius * combinedRadius);

		float discriminant = (B * B) - (4 * A * C);

		float t = (-B - sqrt(discriminant)) / (2 * A);
		if (t < 0)
		  t = (-B + sqrt(discriminant)) / (2 * A);

		if (t >= 0)
		{
		  return t;
		}
		else
		{
		  return -1.0f;
		}
	}
		break;
	case Collider::COLLIDER_BOX:
	{
		Vector3 w0 = b->pos;
		Vector3 b1 = this->pos;
		Vector3 N = b->front;
		Vector3 dir = w0 - b1;
		if (dir.Dot(N) < 0)
			return -1;

		Vector3 U = this->vel;
		float r = this->scale.x;
		float h = b->scale.x;

		w0 -= (r + h * 0.5f) * N;

		float dist = (w0 - b1).Dot(N);
		float speed = U.Dot(N);
		float th = dist / speed;

		Vector3 NP = Vector3(-N.y, N.x, N.z);

		float l = b->scale.y;
		Vector3 w1 = w0 + (l * 0.5f) * NP;
		Vector3 w2 = w0 - (l * 0.5f) * NP;

		Vector3 bh = b1 + U * th;

		if ((w1 - bh).Dot(w2 - bh) > 0)
			return -1;

		return th;
	}
		break;

	default:
		return -1;
		break;
	}
}

void GameObject::CollisionResponse(GameObject* b)
{
	if (this->collider.type != Collider::COLLIDER_BALL)
		return;

	switch (b->collider.type)
	{
	case Collider::COLLIDER_BALL:
	{//BALL TO BALL
		Vector3 N = (this->pos - b->pos).Normalized();

		Vector3 u1N = this->vel.Dot(N) * N;
		Vector3 u2N = b->vel.Dot(N) * N;

		if (!b->GetCollider().isStatic)
			b->vel = b->vel + (((2 * this->mass) / (this->mass + b->mass)) * (u1N - u2N));

		if (!this->collider.isStatic)
		{
			this->vel = this->vel + (((2 * b->mass) / (this->mass + b->mass)) * (u2N - u1N));

			float distanceSquared = (this->pos - b->pos).LengthSquared();
			float combinedRadius = this->scale.x + b->scale.x;
			float difference = distanceSquared - (combinedRadius * combinedRadius);

			this->vel -= N * difference;
		}
	}
		break;

	case Collider::COLLIDER_BOX:
	{
		Vector3 N = b->front;
		Vector3 ab = b->pos - this->pos;

		if (abs(ab.Dot(N)) < b->scale.x * 0.5f)
		{
		   Vector3 NP = Vector3(-N.y, N.x, N.z);
		   if (ab.Dot(NP) < 0)
			   NP = -NP;
		   if (this->vel.Dot(NP) < 0)
			   return;

		   this->vel = this->vel - ((2 * this->vel).Dot(NP)) * NP;

		   float difference = ab.Dot(NP) - (b->scale.x * 0.5f + this->scale.x);
		   this->pos -= NP * difference;
		}
		else
		{
		   if (ab.Dot(N) < 0)
			   N = -N;
		   if (this->vel.Dot(N) < 0)
			   return;

		   this->vel = this->vel - ((2 * this->vel).Dot(N)) * N;

		   float difference = ab.Dot(N) - (b->scale.y * 0.5f + this->scale.x);
		   this->pos -= N * difference;
		}
	}
		break;

	default:break;
	}
}

void GameObject::HandleInteraction(GameObject* b, double dt)
{
	if (CheckCollision(b, dt))
	{
		CollisionResponse(b);
	}
}

Vector3 GameObject::GetPosition()
{
	return this->pos;
}

Vector3 GameObject::GetVelocity()
{
	return this->vel;
}

Vector3 GameObject::GetFront()
{
	return this->front;
}

Vector3 GameObject::GetScale()
{
	return this->scale;
}

GameObject::GAMEOBJECT_TYPE GameObject::GetType()
{
	return this->type;
}

Collider GameObject::GetCollider()
{
	return this->collider;
}

bool GameObject::IsActive()
{
	return this->active;
}

float GameObject::GetMass()
{
	return this->mass;
}

GameObject::TEAM_TYPE GameObject::GetTeam()
{
	return team;
}

void GameObject::SetPostion(float x, float y, float z)
{
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;
}

void GameObject::SetPostion(Vector3 pos)
{
	this->pos = pos;
}

void GameObject::SetVelocity(float x, float y, float z)
{
	this->vel.x = x;
	this->vel.y = y;
	this->vel.z = z;
}

void GameObject::SetVelocity(Vector3 vel)
{
	this->vel = vel;
}

void GameObject::SetScale(float x, float y, float z)
{
	this->scale.x = x;
	this->scale.y = y;
	this->scale.z = z;
}

void GameObject::SetScale(Vector3 scale)
{
	this->scale = scale;
}

void GameObject::SetFront(float x, float y, float z)
{
	this->front.x = x;
	this->front.y = y;
	this->front.z = z;
}

void GameObject::SetFront(Vector3 front)
{
	this->front = front;
}

void GameObject::SetType(GAMEOBJECT_TYPE type)
{
	this->type = type;
}

void GameObject::SetColliderType(Collider::COLLIDER_TYPE type)
{
	this->collider.type = type;
}

void GameObject::SetMass(float mass)
{
	this->mass = mass;
}

void GameObject::SetActive(bool active)
{
	this->active = active;
}

void GameObject::SetTeam(TEAM_TYPE team)
{
	this->team = team;
}

/*
TEMPLATE FOR COLLISION CHECK OVERLOADING

This is an example for Player Projectile
This allows player bullets to skip checking against other projectiles


void GameObject::CollisionResponse(GameObject* b)
{
	//this is for skipping checks
	if( b->type == PLAYER_PROJECTILE ||
		b->type == ENEMY_PROJECTILE 
		)
		{
			return;
		}

	//This is for if you want default physics check
	GameObject::CollisionCheck(b);
}


*/

/*
TEMPLATE FOR COLLISION RESPONSE OVERLOADING

This is an example for Player Projectile


void GameObject::CollisionResponse(GameObject* b)
{
	//This is for if you want default physics response
	GameObject::CollisionResponse(b); 

	//remove this player projectile
	this->active = false; 

	//this is for special interactions
	switch(b->type) 
	{
		case SNAKE_HEAD:
				b->HP -= 10;//deals damage if it hits boss
			break;
		case SNAKE_BODY:
				b->HP -= 5;//deals half damage to body
			break;

		default:
			break;
	}
}


*/