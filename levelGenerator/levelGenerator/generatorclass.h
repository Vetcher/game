#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include <fstream>

typedef unsigned int uint;
#define TERRAIN_LENGTH 1.0f

#define COLOR_DEF 210, 210, 210
#define COLOR_0 230, 230, 230
#define COLOR_1 255, 0, 0
#define COLOR_2 0, 255, 0
#define COLOR_3 0, 0, 255
#define COLOR_4 255, 255, 0
#define COLOR_5 255, 0, 255
#define COLOR_6 0, 255, 255

struct Vertex
{
	unsigned char r, g, b, a;
	float x, y, z;
};

class generatorclass
{
private:
	std::vector<std::vector<uint>> _field;
	HWND* _hwnd;
	uint WINWIDTH = 640;
	uint WINHEIGHT = 480;
	HDC	_hDC = NULL;
	HGLRC _hRC = NULL;
#define MAX_ZOOM -1.0f
#define MIN_ZOOM -30.0f
	float _current_zoom = -10.0f;

	void CreateSettings();
	void LoadSettings();
	void LoadLevel(const char* filename);
public:
	uint _current_view_x = 0;
	uint _current_view_y = 0;
	int _width;
	int _height;

	generatorclass(HWND* hwnd);
	~generatorclass();
	void ChangeCurrent(uint);
	void render();
	void generatorclass::ChangeZoom(short wheel_delta);
	void save();
};

