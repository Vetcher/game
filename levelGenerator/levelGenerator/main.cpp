#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "generatorclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool ChangeWindowSize(const uint WINWIDTH, const uint WINHEIGHT)
{
	if (WINWIDTH && WINHEIGHT)
	{
		glViewport(0, 0, WINWIDTH, WINHEIGHT);	// Вьюпорт OpenGL

		glMatrixMode(GL_PROJECTION);		// Матрица проекции
		glLoadIdentity();					// Сбрасываем

		// Устанавливаем перспективу OpenGL (угол взгляда, аспект ширины и высоты, ближайшее и дальнее отсечение взгляда)
		gluPerspective(45.0, (float)WINWIDTH / (float)WINHEIGHT, 0.1, 100.0);
	}
	return false;
}

int WINAPI WinMain(HINSTANCE hInstance,
						 HINSTANCE hPrevInstance,
						 LPSTR     lpCmdLine,
						 int       nCmdShow)
{
	WNDCLASSEX winClass;
	MSG        uMsg;

	HWND  hWnd = NULL;

	memset(&uMsg, 0, sizeof(uMsg));

	winClass.lpszClassName = "lvlgenerator";
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc = WindowProc;
	winClass.hInstance = hInstance;
	winClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	winClass.hIconSm = NULL;
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName = NULL;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;

	if (!RegisterClassEx(&winClass))
		return E_FAIL;

	hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
									"lvlgenerator",
									"level generator",
									WS_OVERLAPPEDWINDOW | WS_VISIBLE,								// window
									//WS_POPUP,																// fullscreen
									0, 0, 640, 480, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
		return E_FAIL;

	//ShowWindow(hWnd, nCmdShow);
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hWnd);

	generatorclass gen(&hWnd);
	HWND desktop = GetDesktopWindow();
	RECT screenSize;
	GetWindowRect(desktop, &screenSize);
	ChangeWindowSize(screenSize.right, screenSize.bottom);

	while (uMsg.message != WM_QUIT)
	{
		if (PeekMessage(&uMsg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&uMsg);

			switch (uMsg.message)
			{
				case WM_KEYDOWN:
				{
					switch (uMsg.wParam)
					{
						case 0x31:
						case 0x32:
						case 0x33:
						case 0x34:
						case 0x35:
						case 0x36:
						case 0x37:
						case 0x38:
						case 0x39:
						{
							gen.ChangeCurrent(uMsg.wParam - 0x31);
							break;
						}
						case 0x45:
						{
							gen.save();
							break;
						}
						case 0x44:
						case VK_RIGHT:
						{
							if (gen._current_view_x < gen._width - 1)
								gen._current_view_x++;
							break;
						}
						case 0x41:
						case VK_LEFT:
						{
							if (gen._current_view_x > 0)
								gen._current_view_x--;
							break;
						}
						case 0x57:
						case VK_UP:
						{
							if (gen._current_view_y < gen._height - 1)
								gen._current_view_y++;
							break;
						}
						case 0x53:
						case VK_DOWN:
						{
							if (gen._current_view_y > 0)
								gen._current_view_y--;
							break;
						}
						case VK_ESCAPE:
						{
							PostQuitMessage(0);
							break;
						}
						default:
							break;
					}
					break;
				}
				case WM_SIZE: // Изменение размера окна
				{
					ChangeWindowSize(LOWORD(uMsg.lParam), HIWORD(uMsg.lParam));
					break;
				}
				case WM_MOUSEWHEEL:
				{
					gen.ChangeZoom(GET_WHEEL_DELTA_WPARAM(uMsg.wParam));
					break;
				}
				default:
					break;
			}

			DispatchMessage(&uMsg);
		}
		else
		{
			gen.render();
		}
	}

	UnregisterClass("lvlgenerator", winClass.hInstance);

	return uMsg.wParam;
}

LRESULT CALLBACK WindowProc(HWND   hWnd,
									 UINT   msg,
									 WPARAM wParam,
									 LPARAM lParam)
{
	switch (msg)
	{
		// Если закрыли окно - выходим 
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}

			// Если уничтожили окно - выходим
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_SIZE: // Изменение размера окна
		{
			ChangeWindowSize(LOWORD(lParam), HIWORD(lParam));
			break;
		}
			// Умолчание
		default:
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	return 0;
}