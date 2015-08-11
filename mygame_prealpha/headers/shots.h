//#pragma once
#ifndef SHOTS_H
#define SHOTS_H

#include "resource.h"
#include "server.h"

#define MAXRANGE 10.0f

class Server;

class BaseShot
{
public:
	const shot::SHOT_TYPE type = shot::FIREBALL;
	float dir_x = 1.0f;
	float dir_y = 1.0f;
	float radius = 0.01f;
	const float speed = 0.05f;
	uint owner;
	int dmg = 0;
	uint parameter = 0;
	bool _isAlive = true;
public:
	friend class Server;
	float distance = MAXRANGE;
	float x;
	float y;
	BaseShot();
	BaseShot(float p_x, float p_y, float d_x, float d_y, int damage, uint own);
	uint GetDamage() const
	{
		return dmg;
	}
	virtual void UpdateShot();
	virtual void DrawIt();
};

#endif // SHOTS_H