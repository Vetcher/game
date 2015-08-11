//#pragma once
#ifndef RENDERCLASS_H
#define RENDERCLASS_H

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <map>
#include <list>

#include "player_interface.h"
#include "server.h"

class BaseShot;
class Server;

class GL_Painter
{
private:
	//start window properties
	uint WINWIDTH;
	uint WINHEIGHT;

	HWND* _hWnd;
	HDC	_hDC = NULL;
	HGLRC _hRC = NULL;

	float _current_zoom;
#define MAX_ZOOM -10.0f
#define MIN_ZOOM -20.0f

	bool fullscreen;

	// TODO добавить класс с моделями объектов

	uint _mainUnitId;
public:
	LONG cur_mouse_pos_x;
	LONG cur_mouse_pos_y;
	bool keys[256];
	bool mousekeys[16];
	Server mainserver;
	friend class Server;

	GL_Painter(HWND* g_hWnd);
	~GL_Painter();
	void ChangeFullScreen();
	void Render();
	void ChangeZoom(short);
	bool ChangeWindowSize(uint width, uint height);

	void ServerChangeUnit(const UnitData&);
	void AddUnit(const UnitData&);
	void SetMainUnit(uint);

	void PlayerMouseMove(LONG x, LONG y)
	{
		cur_mouse_pos_x = x;
		cur_mouse_pos_y = y;
		mainserver._units.at(_mainUnitId)->cur_point_to_view_x = ((float)x - WINWIDTH / 2) * 0.00789473684f;
		mainserver._units.at(_mainUnitId)->cur_point_to_view_y = ((float)y - WINHEIGHT / 2) * 0.00789473684f;
	}
	void PlayerLBDown(LONG x, LONG y)
	{
		cur_mouse_pos_x = x;
		cur_mouse_pos_y = y;
		float xcl = ((float)x - (float)WINWIDTH / 2) * 0.00789473684f + mainserver._units.at(_mainUnitId)->data.x;
		float ycl = ((float)WINHEIGHT / 2 - (float)y) * 0.00789473684f + mainserver._units.at(_mainUnitId)->data.y;
		//mainserver.Hero->_isShooting = true;
		mainserver.PlayerClick(0, xcl, ycl);
	}
	void PlayerLBUp()
	{
		//mainserver.Hero->_isShooting = false;
	}
	void PlayerRBDown(LONG x, LONG y)
	{
		cur_mouse_pos_x = x;
		cur_mouse_pos_y = y;
		float xcl = ((float)x - (float)WINWIDTH / 2) * 0.00789473684f + mainserver._units.at(_mainUnitId)->data.x;
		float ycl = ((float)WINHEIGHT / 2 - (float)y) * 0.00789473684f + mainserver._units.at(_mainUnitId)->data.y;
		mainserver.PlayerClick(0, xcl, ycl);
	}
};

#endif // RENDERCLASS_H