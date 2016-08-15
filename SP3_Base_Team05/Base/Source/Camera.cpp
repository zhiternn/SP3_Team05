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
}

void Camera::Update(double dt)
{
}