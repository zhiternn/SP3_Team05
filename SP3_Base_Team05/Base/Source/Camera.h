#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include <vector>

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);

	float m_worldWidth, m_worldHeight;

	void Include(Vector3 *pos);
	
private:
	//Vector List of entities taken into account for camera position
	std::vector<Vector3*> entityList;
};

#endif