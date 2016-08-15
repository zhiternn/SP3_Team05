#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

Camera::Camera() :
position(Vector3(0, 0, 1)),
target(Vector3(0, 0, 0)),
up(Vector3(0, 1, 0))
{
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;

	m_worldHeight = 100.0f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
}

void Camera::Update(double dt)
{
	//Retrieve Entities from vector<Vector3>entityList and update the camera position based off their average pos
	Vector3 cameraTarget;
	//set first entity as default position

	for (int i = 0; i < entityList.size(); i++)
	{
		//Average it out :3
		cameraTarget.x += (entityList[i]->x);
		cameraTarget.y += (entityList[i]->y);
	}
	if (entityList.size() > 0)
	{
		cameraTarget.x /= entityList.size();
		cameraTarget.y /= entityList.size();
	}

	this->target = cameraTarget;
	this->position = this->target;
	this->position.z += 1;
}

void Camera::Include(Vector3 *pos)
{
	entityList.push_back(pos);
}