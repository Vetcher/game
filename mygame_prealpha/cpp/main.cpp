#define VERSION "prealpha"

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <ctime>
#include <stdexcept>

//	my includes
#include "../headers/resource.h"
#include "../headers/server.h"
#include "../headers/player_interface.h"
#include "../headers/renderclass.h"

#define PLAYER_LEFT_BUTTON_TIMER 0x10
#define PLAYER_LEFT_BUTTON_DELAY 200
#define PLAYER_RIGHT_BUTTON_TIMER 0x11
#define PLAYER_RIGHT_BUTTON_DELAY 1000

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void SetFullScreen();
void InitGL(HWND&, HDC&, HGLRC&);
void RenderScene(HDC&);
void ShutDownGL(HWND&, HDC&, HGLRC&);

int WINAPI WinMain(HINSTANCE hInstance,
						 HINSTANCE hPrevInstance,
						 LPSTR     lpCmdLine,
						 int       nCmdShow)
{
	WNDCLASSEX winClass;
	MSG        uMsg;

	HWND  g_hWnd = NULL;

	memset(&uMsg, 0, sizeof(uMsg));

	winClass.lpszClassName = "Mygame";
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc = WindowProc;
	winClass.hInstance = hInstance;
	winClass.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
	winClass.hIconSm = LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName = NULL;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;

	if (!RegisterClassEx(&winClass))
		return E_FAIL;

	g_hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
									"Mygame",
									VERSION,
									WS_OVERLAPPEDWINDOW | WS_VISIBLE,								// window
									//WS_POPUP,																// fullscreen
									0, 0, 640, 480, NULL, NULL, hInstance, NULL);
	
	if (g_hWnd == NULL)
		return E_FAIL;

	//ShowWindow(g_hWnd, nCmdShow);
	ShowWindow(g_hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(g_hWnd);

	srand((uint)time(0));

	// создание таймеров
	//SetTimer(g_hWnd, 1, 2000, 0);

	try
	{
		GL_Painter painter(&g_hWnd);
		painter.ChangeFullScreen();
		painter.mainserver.LoadLevel("level.txt");
		// массив дл€ отслеживани€ нажатий клавиш

		// обработка сообщений
		while (uMsg.message != WM_QUIT)
		{
			painter.mainserver.UpdateAllObjects();
			if (PeekMessage(&uMsg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&uMsg);

				// обработка сообщений дл€ рисовальщика и сервера

				switch (uMsg.message)
				{
						// обработка нажати€ клавиши
					case WM_KEYDOWN:
					{
						painter.keys[uMsg.wParam] = true;
						switch (uMsg.wParam)
						{
							// ≈сли нажали Esc - выходим
							case VK_ESCAPE:
							{
								PostQuitMessage(0);
								break;
							}
						}
						if (uMsg.wParam == 0x57)
						{
							painter.mainserver.InputCommand(SERVER_PLAYER_MOVE_TOP, 0);
						}
						if (uMsg.wParam == 0x53)
						{
							painter.mainserver.InputCommand(SERVER_PLAYER_MOVE_BOTTOM, 0);
						}
						if (uMsg.wParam == 0x44)
						{
							painter.mainserver.InputCommand(SERVER_PLAYER_MOVE_RIGHT, 0);
						}
						if (uMsg.wParam == 0x41)
						{
							painter.mainserver.InputCommand(SERVER_PLAYER_MOVE_LEFT, 0);
						}
						break;
					}

					case WM_KEYUP:
					{
						painter.keys[uMsg.wParam] = false;
						switch (uMsg.wParam)
						{
							case 0x57:
								painter.mainserver.InputCommand(SERVER_PLAYER_MOVE_STOP_TOP, 0);
								break;
							case 0x53:
								painter.mainserver.InputCommand(SERVER_PLAYER_MOVE_STOP_BOTTOM, 0);
								break;
							case 0x44:
								painter.mainserver.InputCommand(SERVER_PLAYER_MOVE_STOP_RIGHT, 0);
								break;
							case 0x41:
								painter.mainserver.InputCommand(SERVER_PLAYER_MOVE_STOP_LEFT, 0);
								break;
							default:
								break;
						}
						break;
					}

						// обработка действий мыши
					case WM_LBUTTONDOWN:
					{
						SetTimer(g_hWnd, PLAYER_LEFT_BUTTON_TIMER, PLAYER_LEFT_BUTTON_DELAY, 0);
						painter.mousekeys[0x0] = true;
						painter.PlayerLBDown(uMsg.pt.x, uMsg.pt.y);
						break;
					}
					case WM_LBUTTONUP:
					{
						KillTimer(g_hWnd, PLAYER_LEFT_BUTTON_TIMER);
						painter.mousekeys[0x0] = false;
						painter.PlayerLBUp();
						break;
					}
					case WM_RBUTTONDOWN:
					{
						SetTimer(g_hWnd, PLAYER_RIGHT_BUTTON_TIMER, PLAYER_RIGHT_BUTTON_DELAY, 0);
						painter.mousekeys[0x1] = true;
						painter.PlayerRBDown(uMsg.pt.x, uMsg.pt.y);
						break;
					}
					case WM_RBUTTONUP:
					{
						KillTimer(g_hWnd, PLAYER_RIGHT_BUTTON_TIMER);
						painter.mousekeys[0x1] = false;
						break;
					}
					case WM_MOUSEMOVE:
					{
						painter.PlayerMouseMove(uMsg.pt.x, uMsg.pt.y);
						break;
					}
					case WM_MOUSEWHEEL:
					{
						painter.ChangeZoom(GET_WHEEL_DELTA_WPARAM(uMsg.wParam));
						break;
					}
						
					case WM_TIMER: // обработка таймеров
					{
						if (uMsg.hwnd == g_hWnd)
						{
							switch (uMsg.wParam)
							{
								case 1:
									painter.mainserver.AddEnemy((float)(rand() % 5 + 5) / (float)(rand() % 4 + 1), (float)(rand() % 5 + 5) / (float)(rand() % 4 + 1), UNIT_ENEMY1);
									break;
								case PLAYER_LEFT_BUTTON_TIMER:
									painter.PlayerLBDown(painter.cur_mouse_pos_x, painter.cur_mouse_pos_y);
									break;
								case PLAYER_RIGHT_BUTTON_TIMER:
									painter.PlayerRBDown(painter.cur_mouse_pos_x, painter.cur_mouse_pos_y);
									break;
								default:
									break;
							}
						}
						break;
					}
						
					case WM_SIZE: // »зменение размера окна
					{
						painter.ChangeWindowSize(LOWORD(uMsg.lParam), HIWORD(uMsg.lParam));
						break;
					}
					default:
						break;
				}

				DispatchMessage(&uMsg); // обработка оконных действий WindowProc
			}
			else
			{
				painter.Render();
			}
		}
	}
	catch (std::exception error)
	{
		MessageBox(NULL, TEXT(error.what()), "EXPECTED ERROR", MB_OK | MB_ICONINFORMATION);
	}
	catch (...)
	{
		MessageBox(NULL, "Core error, please contact developer.\nvk.com/vetcher\nvetcher@ngs.ru", "UNEXPECTED ERROR", MB_OK | MB_ICONINFORMATION);
	}

	// удаление таймеров
	//KillTimer(g_hWnd, 1);

	UnregisterClass("Mygame", winClass.hInstance);

	return uMsg.wParam;
}

LRESULT CALLBACK WindowProc(HWND   hWnd,
									 UINT   msg,
									 WPARAM wParam,
									 LPARAM lParam)
{
	switch (msg)
	{
			// обработка сообщени€ о таймере
			// ≈сли закрыли окно - выходим 
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}

			// ≈сли уничтожили окно - выходим
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

			// ”молчание
		default:
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	return 0;
}