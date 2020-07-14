#pragma once
struct ClientPlayer
{
	float x = 0;
	float y = 0;
	float radius = 20;
	bool isColliding(float otherX, float otherY) const;
	bool isColliding(const ClientPlayer& other) const;

};

