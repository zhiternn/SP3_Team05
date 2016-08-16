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

	//null check
	if (!entityList.empty())
	{
		//Calculate Camera Position
		for (int i = 0; i < entityList.size(); i++)
		{
			//Add all the positions
			cameraTarget.x += (entityList[i]->x);
			cameraTarget.y += (entityList[i]->y);
		}
		//Average the positions
		cameraTarget.x /= entityList.size();
		cameraTarget.y /= entityList.size();

		std::cout << cameraTarget << std::endl;
        //Check if player is within camera deadzone
        if (Deadzone(entityList[0], cameraTarget))
        {
			//if so, update the camera position
            this->target = cameraTarget;
            this->position = this->target;
            this->position.z += 1;
        }
    }
}

void Camera::Include(Vector3* pos)
{
	entityList.push_back(pos);
}

Vector3 Camera::GetPosition()
{
	return this->position;
}
bool Camera::Deadzone(Vector3 *pos, Vector3 cameraTarget)
{
	//Create a deadzone
	Vector3 upperRight;
	Vector3 bottomLeft;

	//They call me the mother of hardcoding hehe xd
	//Set the upperRight and bottomLeft Bounds
	upperRight.x = cameraTarget.x + 60;
	upperRight.y = cameraTarget.y + 40;

	bottomLeft.x = cameraTarget.x - 60;
	bottomLeft.y = cameraTarget.y - 40;

	//Position check
	if (pos->x < upperRight.x && pos->x > bottomLeft.x)
	{
		if (pos->y < upperRight.y && pos->y > bottomLeft.y)
		{
			//Player is within bounds
			//isDeadzoned = false;
			return true;
		}
		//Player is out of bounds
		else
		{
			//isDeadzoned = true;
			return false;		
		}
	}
	//player is out of bounds
	else
	{
		//isDeadzoned = true;
		return false;
	}
	//if (isDeadzoned)
	//{
	//	//store cameratarget to prevent it from updating
	//}
}