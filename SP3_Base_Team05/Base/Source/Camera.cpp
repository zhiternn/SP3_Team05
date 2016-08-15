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
	Vector3 *cameraPos;
	//set first entity as default position
	if (!entityList.empty())
	{
		cameraPos = entityList[0];

		for (unsigned int i = 1; i < entityList.size(); i++)
		{
			//Average it out :3

			cameraPos->x = ((cameraPos->x + entityList[i]->x) / 2);
			cameraPos->y = ((cameraPos->y + entityList[i]->y) / 2);
		}

		this->position.Set((cameraPos->x, cameraPos->y, cameraPos->z));
		this->target.Set((cameraPos->x, cameraPos->y, cameraPos->z));
		//Check for [Zero Value]
	}
}