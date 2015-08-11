//#pragma once
#ifndef GAME_PLAYER_UNITS_H
#define GAME_PLAYER_UNITS_H

#include <Windows.h>

#include "resource.h"
//#include "server.h"

float UNITRADIUS(const UNIT_TYPE&);

// basic unit class
class Unit
{
protected:
	int move_acceleration_left, move_acceleration_right;
	int move_acceleration_top, move_acceleration_bottom;
	

public:
	friend class Server;
	UnitData data;
	float cur_point_to_view_x, cur_point_to_view_y;

	Unit(uint id = 0, UNIT_TYPE type = UNIT_NULL, uint owner = 0, float x = 0, float y = 0);
	virtual ~Unit();

	virtual void DrawIt()
	{

	}

	void MoveLeft();
	void MoveRight();
	void MoveTop();
	void MoveBottom();

	void MoveStopLeft()
	{
		move_acceleration_left = 0;
		//MessageBox(NULL, L"StopY", L"StopY", MB_OK | MB_ICONINFORMATION);
	}

	void MoveStopRight()
	{
		move_acceleration_right = 0;
		//MessageBox(NULL, L"StopX", L"StopX", MB_OK | MB_ICONINFORMATION);
	}

	void MoveStopBottom()
	{
		move_acceleration_bottom = 0;
		//MessageBox(NULL, L"StopY", L"StopY", MB_OK | MB_ICONINFORMATION);
	}

	void MoveStopTop()
	{
		move_acceleration_top = 0;
		//MessageBox(NULL, L"StopX", L"StopX", MB_OK | MB_ICONINFORMATION);
	}

	int GetAccelerationVertical() const
	{
		return move_acceleration_right;
	}
	int GetAccelerationHorisontal() const
	{
		return move_acceleration_left;
	}
};

// players unit
class Main_Unit : public Unit
{
private:
	
public:
	Main_Unit(uint id, UNIT_TYPE type = UNIT_HERO, uint owner = 0, float x = 0, float y = 0, int health = 0);
	~Main_Unit();
	bool _isShooting = false;
	void DrawIt();
};

class Enemy_Unit : public Unit
{
private:
public:
	Enemy_Unit(uint id, UNIT_TYPE type, uint owner = 0, float x = 0, float y = 0);
	~Enemy_Unit();
	void DrawIt();
};

#endif // GAME_PLAYER_UNITS_H