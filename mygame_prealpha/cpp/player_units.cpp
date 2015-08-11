#include "../headers/player_units.h"

#include <gl/GL.h>
#include <gl/GLU.h>

Main_Unit::Main_Unit(uint id, UNIT_TYPE type, uint owner, float x, float y, int health)
{
	data.id = id;
	data.x = x;
	data.y = y;
	data.type = type;
	data.owner = owner;
	data.radius = UNITRADIUS(type);
	data.health = health;
	data.max_health = health;
	move_acceleration_top = 0;
	move_acceleration_bottom = 0;
	move_acceleration_left = 0;
	move_acceleration_right = 0;
}

Main_Unit::~Main_Unit()
{

}

Enemy_Unit::Enemy_Unit(uint id, UNIT_TYPE type, uint owner, float x, float y)
{
	data.id = id;
	data.x = x;
	data.y = y;
	data.type = type;
	data.owner = owner;
	data.radius = UNITRADIUS(type);
	move_acceleration_top = 0;
	move_acceleration_bottom = 0;
	move_acceleration_left = 0;
	move_acceleration_right = 0;
}

Enemy_Unit::~Enemy_Unit()
{

}

Unit::Unit(uint id, UNIT_TYPE type, uint owner, float x, float y)
{
	data.id = id;
	data.x = x;
	data.y = y;
	data.type = type;
	data.owner = owner;
	data.radius = UNITRADIUS(type);
	move_acceleration_top = 0;
	move_acceleration_bottom = 0;
	move_acceleration_left = 0;
	move_acceleration_right = 0;
}

Unit::~Unit()
{

}

#define MOVEacceleration_LIMIT 3

void Unit::MoveLeft()
{
	//if (move_acceleration_horisontal > -MOVEacceleration_LIMIT)
		move_acceleration_left = 1;
	//MessageBox(NULL, L"Left", L"Left", MB_OK | MB_ICONINFORMATION);
}

void Unit::MoveRight()
{
	//if (move_acceleration_horisontal < MOVEacceleration_LIMIT)
		move_acceleration_right = 1;
	//MessageBox(NULL, L"Right", L"Right", MB_OK | MB_ICONINFORMATION);
}

void Unit::MoveTop()
{
	//if (move_acceleration_vertical < MOVEacceleration_LIMIT)
		move_acceleration_top = 1;
	//MessageBox(NULL, L"Top", L"Top", MB_OK | MB_ICONINFORMATION);
}

void Unit::MoveBottom()
{
	//if (move_acceleration_vertical > -MOVEacceleration_LIMIT)
		move_acceleration_bottom = 1;
	//MessageBox(NULL, L"Bottom", L"Bottom", MB_OK | MB_ICONINFORMATION);
}

void Main_Unit::DrawIt()
{
	drawCircle(data.x, data.y, data.radius, 100, 240, 240, 240);

	glPushMatrix();

	glTranslatef(data.x, data.y, 0);
	glRotatef(ANGLE(cur_point_to_view_x, cur_point_to_view_y), 0, 0, 1);

	// оружее
	const Vertex weapon[] =
	{
		{ 0, 0, 0, 255, 0.03f, +0.26f, 0.0f },
		{ 0, 0, 0, 255, -0.03f, +0.26f, 0.0f },
		{ 0, 0, 0, 255, -0.03f, 0.0f, 0.0f },
		{ 0, 0, 0, 255, 0.03f, 0.0f, 0.0f },
	};
	glInterleavedArrays(GL_C4UB_V3F, 0, weapon);
	glDrawArrays(GL_QUADS, 0, 4);

	if (_isShooting)
	{
		const Vertex shoot[] =
		{
			{ 230, 230, 0, 255, 0.03f, +0.34f, 0.0f },
			{ 230, 230, 0, 255, -0.03f, +0.34f, 0.0f },
			{ 230, 0, 0, 255, -0.02f, 0.26f, 0.0f },
			{ 230, 0, 0, 255, 0.02f, 0.26f, 0.0f },
		};
		glInterleavedArrays(GL_C4UB_V3F, 0, shoot);
		glDrawArrays(GL_POLYGON, 0, 4);
	}
	else
	{
		const Vertex silenser[] =
		{
			{ 0, 0, 0, 255, 0.03f, +0.27f, 0.0f },
			{ 0, 0, 0, 255, -0.03f, +0.27f, 0.0f },
			{ 0, 0, 0, 255, -0.03f, 0.26f, 0.0f },
			{ 0, 0, 0, 255, 0.03f, 0.26f, 0.0f },
		};
		glInterleavedArrays(GL_C4UB_V3F, 0, silenser);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	glPopMatrix();
	// тело
	drawCircleFill(0.0f, 0.0f, 0.17f, 50, 40, 40, 255);

}

void Enemy_Unit::DrawIt()
{

	// хп бар
	const Vertex healthBar[] =
	{
		{ 57, 191, 0, 230, -(float)data.health / data.max_health * 0.5f, 1.1f, 0.0f },
		{ 96, 193, 54, 230, -(float)data.health / data.max_health* 0.5f, 1.0f, 0.0f },
		{ 113, 224, 62, 230, (float)data.health / data.max_health* 0.5f, 1.0f, 0.0f },
		{ 71, 224, 0, 230, (float)data.health / data.max_health* 0.5f, 1.1f, 0.0f },
	};
	glInterleavedArrays(GL_C4UB_V3F, 0, healthBar);
	glDrawArrays(GL_QUADS, 0, 4);

	// окантовка хп бара
	const Vertex healthBarEdging[] =
	{
		{ 0, 0, 0, 230, -0.5f, 1.1f, 0.0f },
		{ 0, 0, 0, 230, -0.5f, 1.0f, 0.0f },
		{ 0, 0, 0, 230, 0.5f, 1.0f, 0.0f },
		{ 0, 0, 0, 230, 0.5f, 1.1f, 0.0f },
	};
	glInterleavedArrays(GL_C4UB_V3F, 0, healthBarEdging);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	const Vertex model[] =
	{
		{ 255, 40, 70, 200, data.x - data.radius, data.y + data.radius, 0.0f },
		{ 255, 80, 70, 200, data.x - data.radius, data.y - data.radius, 0.0f },
		{ 255, 40, 80, 200, data.x + data.radius, data.y - data.radius, 0.0f },
		{ 255, 40, 40, 200, data.x + data.radius, data.y + data.radius, 0.0f }
	};
	glInterleavedArrays(GL_C4UB_V3F, 0, model);
	glDrawArrays(GL_QUADS, 0, 4);

	//MessageBox(NULL, L"ENEMY", L"...", MB_OK | MB_ICONINFORMATION);



}

float UNITRADIUS(const UNIT_TYPE& t)
{
	switch (t)
	{
		case UNIT_NULL:
			return 0.0f;
		case UNIT_HERO:
			return 0.3f;
		case UNIT_ENEMY1:
			return 0.5f;
		default:
			return 0.0;
	}
}