#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::setPosition(vec3 pos)
{
	position = pos;
}

void Camera::setForward(vec3 front)
{
	forward = normalize(front);
}

void Camera::setUp(vec3 upwards)
{
	up = normalize(upwards);
}

void Camera::translateWorld(vec3 dPos)
{
	position += dPos;
}

void Camera::translateLocal(vec3 strafe)
{
	position += forward*strafe.x + up*strafe.y + getRight()*strafe.z;
}

vec3 Camera::getPosition() const
{
	return position;
}

vec3 Camera::getForward() const
{
	return forward;
}

vec3 Camera::getRight() const
{
	return normalize(cross(forward, up));
}

vec3 Camera::getUp() const
{
	return up;
}

mat4 Camera::getView() const
{
	return lookAt(position, position + forward, up);
}
