#include "generatorclass.h"


generatorclass::generatorclass(HWND* hwnd)
{
	_hwnd = hwnd;
	LoadSettings();
	_field.insert(_field.begin(), _height, std::vector<uint>(_width, 0));
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;

	_hDC = GetDC(*hwnd);
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

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	LoadLevel("level.txt");
}

void generatorclass::LoadLevel(const char* filename)
{
	std::fstream file;
	file.open(filename, std::ios_base::in);

	if (file.is_open())
	{
		float x = 0;
		float y = 0;
		while (true)
		{
			char c;
			file.get(c);
			if (!file.eof())
			{
				if (c == '\n')
				{
					y += 1;
					x = 0;
				}
				else
				{
					_field[x][y] = c - '0';
					x += 1;
				}
			}
			else
			{
				break;
			}
		}
	}

	file.close();
}

void generatorclass::save()
{
	std::fstream file;
	file.open("level.txt", std::ios_base::out | std::ios_base::trunc);

	for (uint i = 0; i != _width; i++)
	{
		for (uint j = 0; j != _height; j++)
		{
			file << _field[j][i];
		}
		file << std::endl;
	}

	file.close();
}

void generatorclass::ChangeCurrent(uint n)
{
	if (_current_view_x >= 0 && _current_view_y >= 0 && _current_view_x <= _width && _current_view_y <= _height)
	_field[_current_view_x][_current_view_y] = n;
}

void generatorclass::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); // Матрица модели 
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, _current_zoom);

	gluLookAt(_current_view_x + TERRAIN_LENGTH / 2, _current_view_y + TERRAIN_LENGTH / 2, 1.0,
				 _current_view_x + TERRAIN_LENGTH / 2, _current_view_y + TERRAIN_LENGTH / 2, 0.0, 0.0, 1.0, 0.0);

	/*for (std::vector<std::vector<uint>>::iterator i = _field.begin(); i != _field.end(); i++)
	{
		for (std::vector<uint>::iterator j = i->begin(); j != i->end() j++)
		{
			Vertex block[] =
			{
				{ 210, 210, 210, 255, i->x, i->y, 0.0f },
				{ 210, 210, 210, 255, i->x + TERRAIN_LENGTH, i->y, 0.0f },
				{ 210, 210, 210, 255, i->x + TERRAIN_LENGTH, i->y + TERRAIN_LENGTH, 0.0f },
				{ 210, 210, 210, 255, i->x, i->y + TERRAIN_LENGTH, 0.0f }
			};
			glInterleavedArrays(GL_C4UB_V3F, 0, block);
			glDrawArrays(GL_QUADS, 0, 4);
		}
	}*/
	for (uint i = 0; i != _width; i++)
	{
		for (uint j = 0; j != _height; j++)
		{
			switch (_field[i][j])
			{
				case 0:
				{
					Vertex block[] =
					{
						{ COLOR_0, 255, i, j, 0.0f },
						{ COLOR_0, 255, i + TERRAIN_LENGTH, j, 0.0f },
						{ COLOR_0, 255, i + TERRAIN_LENGTH, j + TERRAIN_LENGTH, 0.0f },
						{ COLOR_0, 255, i, j + TERRAIN_LENGTH, 0.0f }
					};
					glInterleavedArrays(GL_C4UB_V3F, 0, block);
					glDrawArrays(GL_QUADS, 0, 4);
					break;
				}
				case 1:
				{
					Vertex block[] =
					{
						{ COLOR_1, 255, i, j, 0.0f },
						{ COLOR_1, 255, i + TERRAIN_LENGTH, j, 0.0f },
						{ COLOR_1, 255, i + TERRAIN_LENGTH, j + TERRAIN_LENGTH, 0.0f },
						{ COLOR_1, 255, i, j + TERRAIN_LENGTH, 0.0f }
					};
					glInterleavedArrays(GL_C4UB_V3F, 0, block);
					glDrawArrays(GL_QUADS, 0, 4);
					break;
				}
				case 2:
				{
					Vertex block[] =
					{
						{ COLOR_2, 255, i, j, 0.0f },
						{ COLOR_2, 255, i + TERRAIN_LENGTH, j, 0.0f },
						{ COLOR_2, 255, i + TERRAIN_LENGTH, j + TERRAIN_LENGTH, 0.0f },
						{ COLOR_2, 255, i, j + TERRAIN_LENGTH, 0.0f }
					};
					glInterleavedArrays(GL_C4UB_V3F, 0, block);
					glDrawArrays(GL_QUADS, 0, 4);
					break;
				}
				case 3:
				{
					Vertex block[] =
					{
						{ COLOR_3, 255, i, j, 0.0f },
						{ COLOR_3, 255, i + TERRAIN_LENGTH, j, 0.0f },
						{ COLOR_3, 255, i + TERRAIN_LENGTH, j + TERRAIN_LENGTH, 0.0f },
						{ COLOR_3, 255, i, j + TERRAIN_LENGTH, 0.0f }
					};
					glInterleavedArrays(GL_C4UB_V3F, 0, block);
					glDrawArrays(GL_QUADS, 0, 4);
					break;
				}
				case 4:
				{
					Vertex block[] =
					{
						{ COLOR_4, 255, i, j, 0.0f },
						{ COLOR_4, 255, i + TERRAIN_LENGTH, j, 0.0f },
						{ COLOR_4, 255, i + TERRAIN_LENGTH, j + TERRAIN_LENGTH, 0.0f },
						{ COLOR_4, 255, i, j + TERRAIN_LENGTH, 0.0f }
					};
					glInterleavedArrays(GL_C4UB_V3F, 0, block);
					glDrawArrays(GL_QUADS, 0, 4);
					break;
				}
				case 5:
				{
					Vertex block[] =
					{
						{ COLOR_5, 255, i, j, 0.0f },
						{ COLOR_5, 255, i + TERRAIN_LENGTH, j, 0.0f },
						{ COLOR_5, 255, i + TERRAIN_LENGTH, j + TERRAIN_LENGTH, 0.0f },
						{ COLOR_5, 255, i, j + TERRAIN_LENGTH, 0.0f }
					};
					glInterleavedArrays(GL_C4UB_V3F, 0, block);
					glDrawArrays(GL_QUADS, 0, 4);
					break;
				}
				case 6:
				{
					Vertex block[] =
					{
						{ COLOR_6, 255, i, j, 0.0f },
						{ COLOR_6, 255, i + TERRAIN_LENGTH, j, 0.0f },
						{ COLOR_6, 255, i + TERRAIN_LENGTH, j + TERRAIN_LENGTH, 0.0f },
						{ COLOR_6, 255, i, j + TERRAIN_LENGTH, 0.0f }
					};
					glInterleavedArrays(GL_C4UB_V3F, 0, block);
					glDrawArrays(GL_QUADS, 0, 4);
					break;
				}
				default:
				{
					Vertex block[] =
					{
						{ COLOR_DEF, 255, i, j, 0.0f },
						{ COLOR_DEF, 255, i + TERRAIN_LENGTH, j, 0.0f },
						{ COLOR_DEF, 255, i + TERRAIN_LENGTH, j + TERRAIN_LENGTH, 0.0f },
						{ COLOR_DEF, 255, i, j + TERRAIN_LENGTH, 0.0f }
					};
					glInterleavedArrays(GL_C4UB_V3F, 0, block);
					glDrawArrays(GL_QUADS, 0, 4);
					break;
				}
			}
		}
	}
	Vertex edge[] =
	{
		{ 0, 0, 0, 255, _current_view_x, _current_view_y, 0.0f },
		{ 0, 0, 0, 255, _current_view_x + TERRAIN_LENGTH, _current_view_y, 0.0f },
		{ 0, 0, 0, 255, _current_view_x + TERRAIN_LENGTH, _current_view_y + 0.01f, 0.0f },
		{ 0, 0, 0, 255, _current_view_x, _current_view_y + 0.01f, 0.0f },

		{ 0, 0, 0, 255, _current_view_x, _current_view_y, 0.0f },
		{ 0, 0, 0, 255, _current_view_x + 0.01f, _current_view_y, 0.0f },
		{ 0, 0, 0, 255, _current_view_x + 0.01f, _current_view_y + TERRAIN_LENGTH, 0.0f },
		{ 0, 0, 0, 255, _current_view_x, _current_view_y + TERRAIN_LENGTH, 0.0f },

		{ 0, 0, 0, 255, _current_view_x, _current_view_y + TERRAIN_LENGTH, 0.0f },
		{ 0, 0, 0, 255, _current_view_x, _current_view_y + TERRAIN_LENGTH - 0.01f, 0.0f },
		{ 0, 0, 0, 255, _current_view_x + TERRAIN_LENGTH, _current_view_y + TERRAIN_LENGTH - 0.01f, 0.0f },
		{ 0, 0, 0, 255, _current_view_x + TERRAIN_LENGTH, _current_view_y + TERRAIN_LENGTH, 0.0f },

		{ 0, 0, 0, 255, _current_view_x + TERRAIN_LENGTH, _current_view_y + TERRAIN_LENGTH, 0.0f },
		{ 0, 0, 0, 255, _current_view_x + TERRAIN_LENGTH - 0.01f, _current_view_y + TERRAIN_LENGTH, 0.0f },
		{ 0, 0, 0, 255, _current_view_x + TERRAIN_LENGTH - 0.01f, _current_view_y, 0.0f },
		{ 0, 0, 0, 255, _current_view_x + TERRAIN_LENGTH, _current_view_y, 0.0f },
	};
	glInterleavedArrays(GL_C4UB_V3F, 0, edge);
	glDrawArrays(GL_QUADS, 0, 16);

	SwapBuffers(_hDC);
}

generatorclass::~generatorclass()
{
	if (_hRC != NULL)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(_hRC);
		_hRC = NULL;
	}

	if (_hDC != NULL)
	{
		ReleaseDC(*_hwnd, _hDC);
		_hDC = NULL;
	}
}

void generatorclass::ChangeZoom(short wheel_delta)
{
	if (wheel_delta > 0)
	{
		// wheel up
		if (_current_zoom < MAX_ZOOM)
		{
			_current_zoom += 1.0f;
		}
	}
	else
	{
		// wheel down
		if (_current_zoom > MIN_ZOOM)
		{
			_current_zoom -= 1.0f;
		}
	}
}

void generatorclass::CreateSettings()
{
	std::fstream file;
	file.open("config.ini", std::ios_base::out | std::ios_base::trunc);

	//file << "width =10" << std::endl << "height =10" << std::endl << "outfile =out.txt";
	file << "10 " << "10";

	file.close();
}
void generatorclass::LoadSettings()
{
	std::fstream setfile;
	setfile.open("config.ini", std::ios_base::in);
	if (setfile.is_open())
	{
		//std::string s;
		/*while (!setfile.eof() && !ok)
		{
		char c;
		setfile.get(c);
		if (c != '\n' && c != '\t' && c != ' ')
		s.push_back(c);
		if (c == '\n')
		s.erase();
		if (c == '=')
		{
		if (s == std::string("logmode="))
		{
		setfile.get(c);
		ok = var->setWritelogMode(c - '0');
		}
		if (s == std::string(""))
		}
		}*/

		int w, h;
		setfile >> w >> h;

		_width = w;
		_height = h;

		setfile.close();
	}
	else
	{
		CreateSettings();
		LoadSettings();
	}
}