//#pragma once
#ifndef RESOURCE_H
#define RESOURCE_H

#define IDC_MYICON                      2
#define IDD_OPENGL_DIALOG               102
#define IDD_ABOUTBOX                    103
#define IDS_APP_TITLE                   103
#define IDM_ABOUT                       104
#define IDM_EXIT                        105
#define IDS_HELLO                       106
#define IDI_OPENGL_ICON                 107
#define IDC_OPENGL_MENU                 108
#define IDC_OPENGL_ACCELERATOR          109
#define IDC_OPENGL                      110
#define IDR_MAINFRAME                   128
#define IDC_STATIC                      -1

#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        129
#define _APS_NEXT_COMMAND_VALUE         32771
#define _APS_NEXT_CONTROL_VALUE         1000
#define _APS_NEXT_SYMED_VALUE           110
#endif
#endif

#include <Windows.h>
#include <cmath>

#define PI_f 3.1415926f

typedef unsigned int uint;

// all visible objects inteface
class VisibleObject
{
public:
	virtual void DrawIt() = 0;
};

class Color
{
private:
	uint value;
public:
	Color()
	{
		value = 0x0;
	}
	Color(uint c)
	{
		value = c % 16777216;
	}
	Color(uint r, uint g, uint b)
	{
		value = r << 16 | g << 8 | b;
	}
	~Color()
	{

	}
	void operator=(uint a)
	{
		value = a;
	}
	uint color() const
	{
		return value;
	}
	uint red() const
	{
		return (value >> 16) & 0xFF;
	}
	uint green() const
	{
		return (value >> 8) & 0xFF;
	}
	uint blue() const
	{
		return value & 0xFF;
	}
};

// структура координат
struct Vertex
{
	unsigned char r, g, b, a;
	float x, y, z;
};

enum PlayerUnitColors : uint
{
	color_NEUTRAL = 0xAA6600,
	color_PLAYER1 = 0x3247FF
};

enum UNIT_TYPE : uint
{
	UNIT_NULL = 0u,
	UNIT_HERO,
	UNIT_ENEMY1
};

#define TERRAIN_BLOCK 0u

struct TERRAIN_TYPE
{
	uint type;
	float x;
	float y;
};

float LENGTH(const float x, const float y);

float ANGLE(const float x, const float y);

float LENGTH(const float x1, const float y1, const float x2, const float y2);

struct UnitData
{
	uint id;
	UNIT_TYPE type;
	float x;
	float y;
	int health;
	int max_health;
	uint owner;
	// радиус окружности "объема" юнита
	float radius;
	bool isAlive = true;
};

namespace shot
{

	enum SHOT_TYPE : uint
	{
		FIREBALL = 0
	};

}
void drawCircle(float x, float y, float r, uint amountSegments, Color c);
void drawCircle(float x, float y, float r, uint amountSegments, uint red, uint green, uint blue);
void drawCircleFill(float x, float y, float r, uint amountSegments, Color c);
void drawCircleFill(float x, float y, float r, uint amountSegments, uint red, uint green, uint blue);

#endif // RESOURCE_H