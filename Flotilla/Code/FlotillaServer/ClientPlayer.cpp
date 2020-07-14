#include "ClientPlayer.h"


bool ClientPlayer::isColliding(float otherX, float otherY) const
{

	return radius*radius >= (x-otherX)*(x-otherX) + (y-otherY)*(y-otherY);
}

bool ClientPlayer::isColliding(const ClientPlayer & other) const
{
	return (radius*radius)+(other.radius*other.radius) >= (x - other.x)*(x - other.x) + (y - other.y)*(y - other.y);

}
