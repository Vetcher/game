#include "../headers/renderclass.h"
#include <stdexcept>
#include <vector>

GL_Painter::GL_Painter(HWND* g_hWnd)
{
	// private variables initialization
	_hWnd = g_hWnd;
	WINWIDTH = 640;
	WINHEIGHT = 480;
	_current_zoom = -10.0f;
	fullscreen = false;
	_mainUnitId = 0;

	// gl initialization

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;

	_hDC = GetDC(*g_hWnd);
	GLuint iPixelFormat = ChoosePixelFormat(_hDC, &pfd);

	if (iPixelFormat != 0)
	{
		PIXELFORMATDESCRIPTOR bestMatch_pfd;
		DescribePixelFormat(_hDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd);

		if (bestMatch_pfd.cDepthBits < pfd.cDepthBits)
		{
			return;
		}

		if (SetPixelFormat(_hDC, iPixelFormat, &pfd) == FALSE)
		{
			DWORD dwErrorCode = GetLastError();
			return;
		}
	}
	else
	{
		DWORD dwErrorCode = GetLastError();
		return;
	}

	_hRC = wglCreateContext(_hDC);
	wglMakeCurrent(_hDC, _hRC);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Цвет вьюпорта OpenGL

	ChangeWindowSize(WINWIDTH, WINHEIGHT);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// server initialization
	mainserver.InputCommand(SERVER_START, 0);
	mainserver.ConnectPainter();
}

GL_Painter::~GL_Painter()
{
	for (UNIT_STORAGE_TYPE::iterator i = mainserver._units.begin(); i != mainserver._units.end(); i++)
	{
		delete i->second;
	}
	mainserver._units.clear();

	if (_hRC != NULL)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(_hRC);
		_hRC = NULL;
	}

	if (_hDC != NULL)
	{
		ReleaseDC(*_hWnd, _hDC);
		_hDC = NULL;
	}
}

void GL_Painter::ChangeFullScreen()
{
	HWND desktop = GetDesktopWindow();
	RECT screenSize;
	GetWindowRect(desktop, &screenSize);
	WINWIDTH = screenSize.right;
	WINHEIGHT = screenSize.bottom;
	ChangeWindowSize(WINWIDTH, WINHEIGHT);
}

bool GL_Painter::ChangeWindowSize(const uint width, const uint height)
{
	if (width && height)
	{
		WINWIDTH = width;		// Ширина
		WINHEIGHT = height;		// Высота
		glViewport(0, 0, WINWIDTH, WINHEIGHT);	// Вьюпорт OpenGL

		glMatrixMode(GL_PROJECTION);		// Матрица проекции
		glLoadIdentity();					// Сбрасываем

		// Устанавливаем перспективу OpenGL (угол взгляда, аспект ширины и высоты, ближайшее и дальнее отсечение взгляда)
		gluPerspective(45.0, (float)WINWIDTH / (float)WINHEIGHT, 0.1, 100.0);
	}
	return false;
}

void GL_Painter::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); // Матрица модели 
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, _current_zoom); // Смещение
	//glTranslatef(0.0f, 0.0f, -3.0f); // Смещение

	// загрузка уровня

	gluLookAt(mainserver._units.at(_mainUnitId)->data.x, mainserver._units.at(_mainUnitId)->data.y, 1.0,
				 mainserver._units.at(_mainUnitId)->data.x, mainserver._units.at(_mainUnitId)->data.y, 0.0, 0.0, 1.0, 0.0);

	for (std::list<TERRAIN_TYPE>::iterator i = mainserver._currentlevel.begin(); i != mainserver._currentlevel.end(); i++)
	{
			Vertex block[] =
			{
				{ 210, 210, 210, 255, i->x,						i->y,						0.0f },
				{ 210, 210, 210, 255, i->x + TERRAIN_LENGTH, i->y, 0.0f },
				{ 210, 210, 210, 255, i->x + TERRAIN_LENGTH, i->y + TERRAIN_LENGTH, 0.0f },
				{ 210, 210, 210, 255, i->x, i->y + TERRAIN_LENGTH, 0.0f }
			};
			glInterleavedArrays(GL_C4UB_V3F, 0, block);
			glDrawArrays(GL_QUADS, 0, 4);
	}
	glPushMatrix();
	for (UNIT_STORAGE_TYPE::iterator i = mainserver._units.begin(); i != mainserver._units.end(); i++)
	{
		i->second->DrawIt();
	}
	glPopMatrix();
	for (std::list<BaseShot*>::iterator i = mainserver._shots.begin(); i != mainserver._shots.end(); i++)
	{
		(*i)->DrawIt();
	}

	SwapBuffers(_hDC);
}

void GL_Painter::ChangeZoom(short wheel_delta)
{
	if (wheel_delta > 0)
	{
		// wheel up
		if (_current_zoom < MAX_ZOOM)
		{
			_current_zoom = MAX_ZOOM;
		}
	}
	else
	{
		// wheel down
		if (_current_zoom > MIN_ZOOM)
		{
			_current_zoom = MIN_ZOOM;
		}
	}
}

void GL_Painter::SetMainUnit(uint id)
{
	_mainUnitId = id;
}
