#include "../headers/server.h"
#include <algorithm>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <fstream>

Server::Server()
{
	_IsServerWork = false;
}

bool Server::Start()
{
	if (!_IsServerWork)
	{
		_last_id = 0;
		return true;
	}
	return false;
}

Server::~Server()
{
	ShutDown();
}

bool Server::ShutDown()
{
	if (_IsServerWork)
	{
		// удаление юнитов
		for (std::map<uint, Unit*>::iterator i = _units.begin(); i != _units.end(); i++)
		{
			delete i->second;
		}
		// удаление снарядов
		for (std::list<BaseShot*>::iterator i = _shots.begin(); i != _shots.end(); i++)
		{
			delete *i;
		}
		_players.clear();
		_units.clear();
		return true;
	}
	return false;
}

void Server::UpdateAllObjects()
{
	for (UNIT_STORAGE_TYPE::iterator unit = _units.begin(); unit != _units.end();)
	{
		if (!unit->second->data.isAlive)
		{
			UNIT_STORAGE_TYPE::iterator temp(unit);
			unit++;
			_units.erase(temp);
		}
		else
		{
			unit++;
		}
	}
	for (UNIT_STORAGE_TYPE::iterator unit = _units.begin(); unit != _units.end(); unit++)
	{
		Unit* current_unit = unit->second;
		// рассчёт расстояний между юнитами и стенами
		for (std::list<TERRAIN_TYPE>::iterator j = _currentlevel.begin(); j != _currentlevel.end(); j++)
		{
			if (LENGTH(current_unit->data.x, current_unit->data.y, j->x + TERRAIN_LENGTH / 2.0f, j->y + TERRAIN_LENGTH / 2.0f) <=
				 sqrtf(2.0f * (MOVE_SPEED + current_unit->data.radius + TERRAIN_LENGTH) * (MOVE_SPEED + current_unit->data.radius + TERRAIN_LENGTH)))
			{
				_maybe_face.push_back(std::pair<Unit*, TERRAIN_TYPE*>(current_unit, &(*j)));
			}
		}


		current_unit->data.x += (float)(MOVE_SPEED * current_unit->move_acceleration_right - MOVE_SPEED * current_unit->move_acceleration_left);
		// проверка столкновений со стенами слева/справа
		for (MAYFACE_STORAGE::iterator i = _maybe_face.begin(); i != _maybe_face.end(); i++)
		{
			// пересечение окружности и квадрата/прямоугольника
			// увеличиваем стороны квадрата/прямоугольника на 2 радиуса
			// проверяем, находится ли в новой фигуре центр окружности
			while (IsInRectangle(i->first->data.x, i->first->data.y,
				i->second->x + TERRAIN_LENGTH / 2.0f, i->second->y + TERRAIN_LENGTH / 2.0f,
				TERRAIN_LENGTH + 2.0f * i->first->data.radius, TERRAIN_LENGTH + 2.0f * i->first->data.radius))
			{
				// 1/4 смещение
				i->first->data.x -= (float)(MOVE_SPEED * i->first->move_acceleration_right - MOVE_SPEED * i->first->move_acceleration_left) / 4.0f;
			}
		}
		current_unit->data.y += (float)(MOVE_SPEED * current_unit->move_acceleration_top - MOVE_SPEED * current_unit->move_acceleration_bottom);
		// проверка столкновений со стенами сверху/снизу
		for (MAYFACE_STORAGE::iterator i = _maybe_face.begin(); i != _maybe_face.end(); i++)
		{
			// пересечение окружности и квадрата/прямоугольника
			// увеличиваем стороны квадрата/прямоугольника на 2 радиуса
			// проверяем, находится ли в новой фигуре центр окружности
			while (IsInRectangle(i->first->data.x, i->first->data.y,
				i->second->x + TERRAIN_LENGTH / 2.0f, i->second->y + TERRAIN_LENGTH / 2.0f,
				TERRAIN_LENGTH + 2.0f * i->first->data.radius, TERRAIN_LENGTH + 2.0f * i->first->data.radius))
			{
				// 1/4 смещение
				i->first->data.y -= (float)(MOVE_SPEED * i->first->move_acceleration_top - MOVE_SPEED * i->first->move_acceleration_bottom) / 4.0f;
			}
		}
		// 2 варианта: отправлять всё(после изменений или во время) или только изменения

		// отправляем всё во время
	}
	_maybe_face.clear();
	// удаляем уничтоженные/попавшие снаряды
	if (!_shots.empty())
	{
		for (std::list<BaseShot*>::iterator i = _shots.begin(); i != _shots.end();)
		{
			if ((*i)->_isAlive)
			{
				for (std::list<TERRAIN_TYPE>::iterator j = _currentlevel.begin(); j != _currentlevel.end(); j++)
				{
					if (IsInRectangle((*i)->x, (*i)->y,
						j->x + TERRAIN_LENGTH / 2.0f, j->y + TERRAIN_LENGTH / 2.0f,
						TERRAIN_LENGTH + 2.0f * (*i)->radius, TERRAIN_LENGTH + 2.0f * (*i)->radius))
					{
						(*i)->_isAlive = false;
						break;
					}
				}
				if ((*i)->_isAlive)
				{
					for (UNIT_STORAGE_TYPE::iterator unitpair = _units.begin(); unitpair != _units.end(); unitpair++)
					{
						// проверка на владельцев, союзников не убиваем
						if (unitpair->second->data.owner != (*i)->owner)
						{
							// проверка расстояния между центрами окружностей(юнита и снаряда)
							if (LENGTH(unitpair->second->data.x, unitpair->second->data.y, (*i)->x, (*i)->y) <= unitpair->second->data.radius + (*i)->radius)
							{
								// наносим урон, убиваем юнит, убиваем снаряд
								unitpair->second->data.health -= (*i)->dmg;
								if (unitpair->second->data.health <= 0)
								{
									unitpair->second->data.isAlive = false;
								}
								(*i)->_isAlive = false;
								break;
							}
						}
					}
				}
				(*i)->UpdateShot();	
			}
			if (!(*i)->_isAlive)
			{
				std::list<BaseShot*>::iterator j = i;
				i++;
				_shots.erase(j);
			}
			else
			{
				i++;
			}
		}
	}
}

bool Server::IsInRectangle(const float& x, const float& y, const float& x_c, const float& y_c, const float& a, const float& b)
{
	if (x_c - a / 2.0f <= x && x <= x_c + a / 2.0f && y_c - b / 2.0f <= y && y <= y_c + b / 2.0f)
	{
		return true;
	}
	return false;
}

void Server::CreateLinearShot(shot::SHOT_TYPE type, uint owner, float pos_x, float pos_y,
										float direction_vector_x, float direction_vector_y,
										uint dealing_damage, uint parameter)
{
	_shots.push_back(new BaseShot(pos_x, pos_y, direction_vector_x, direction_vector_y, 10, owner));
}

void Server::ReturnForRender()
{
	
}

void Server::PlayerClick(uint playerid, float x, float y)
{
	CreateLinearShot(shot::FIREBALL, 1, Hero->data.x, Hero->data.y, x - Hero->data.x, y - Hero->data.y, 10, 0);
}

void Server::ConnectPainter()
{
	_players.push_back(0);
	Main_Unit* unit = new Main_Unit(generateID(), UNIT_HERO, 1, 1.5f, 2.5f, 100);
	Hero = unit;
	_units.insert(std::pair<uint, Unit*>(0, unit));
}

bool Server::InputCommand(unsigned char key, unsigned char player_id)
{
	try
	{
		switch (key)
		{
			// service
			case SERVER_SHUTDOWN:
				return ShutDown();
			case SERVER_START:
				return Start();

			// player
			case SERVER_PLAYER_MOVE_BOTTOM:
				_units.at(_players.at(player_id))->MoveBottom();
				return true;
			case SERVER_PLAYER_MOVE_TOP:
				_units.at(_players.at(player_id))->MoveTop();
				return true;
			case SERVER_PLAYER_MOVE_LEFT:
				_units.at(_players.at(player_id))->MoveLeft();
				return true;
			case SERVER_PLAYER_MOVE_RIGHT:
				_units.at(_players.at(player_id))->MoveRight();
				return true;
			case SERVER_PLAYER_MOVE_STOP_RIGHT:
				_units.at(_players.at(player_id))->MoveStopRight();
				return true;
			case SERVER_PLAYER_MOVE_STOP_LEFT:
				_units.at(_players.at(player_id))->MoveStopLeft();
				return true;
			case SERVER_PLAYER_MOVE_STOP_TOP:
				_units.at(_players.at(player_id))->MoveStopTop();
				return true;
			case SERVER_PLAYER_MOVE_STOP_BOTTOM:
				_units.at(_players.at(player_id))->MoveStopBottom();
				return true;
			// default
			default:
				return false;
		}
	}
	catch (...)
	{
		return false;
	}
	return false;
}

void Server::AddEnemy(float x, float y, UNIT_TYPE type)
{
	uint newid = generateID();
	Enemy_Unit* unit = new Enemy_Unit(newid, type, 0, x, y);
	_units.insert(std::pair<uint, Unit*>(newid, unit));
}

uint Server::generateID()
{
	_last_id++;
	while (_units.count(_last_id))
	{
		_last_id++;
	}
	return _last_id;
}

void Server::LoadLevel(const char* filename)
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
					if (c - '0')
					{
						TERRAIN_TYPE tt = { (uint)c, x, y };
						_currentlevel.push_back(tt);
					}
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