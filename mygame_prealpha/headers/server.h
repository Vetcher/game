//#pragma once
#ifndef SERVER_H
#define SERVER_H

#ifndef SERVER_COMMANDS
#define SERVER_COMMANDS

// service commands
#define SERVER_SHUTDOWN 0x00
#define SERVER_START 0x01

// player commands
#define SERVER_PLAYER_MOVE_TOP 0x10
#define SERVER_PLAYER_MOVE_BOTTOM 0x11
#define SERVER_PLAYER_MOVE_LEFT 0x12
#define SERVER_PLAYER_MOVE_RIGHT 0x13
#define SERVER_PLAYER_MOVE_STOP_LEFT 0x14
#define SERVER_PLAYER_MOVE_STOP_RIGHT 0x15
#define SERVER_PLAYER_MOVE_STOP_TOP 0x16
#define SERVER_PLAYER_MOVE_STOP_BOTTOM 0x17

#endif // SERVER_COMMANDS

#include <vector>
#include <map>
#include <list>

#include "player_units.h"
#include "resource.h"
#include "shots.h"
//#include "renderclass.h"

#define TERRAIN_LENGTH 1.0f

#define MOVE_SPEED 0.015f
typedef std::pair<uint, Unit*> UNIT_PAIR;
typedef std::map<uint, Unit*> UNIT_STORAGE_TYPE;
typedef std::pair<Unit*, TERRAIN_TYPE*> MAYFACE_PAIR;
typedef std::list <MAYFACE_PAIR> MAYFACE_STORAGE;

class BaseShot;

class Server
{
private:
	bool _IsServerWork;

	std::vector<uint> _players;
	// <id, pointer to graphic unit>
	UNIT_STORAGE_TYPE _units;
	std::list<BaseShot*> _shots;

	uint _last_id; // for generate

	Main_Unit* Hero;

	// create linear shots, like gun shots or fireball
	void CreateLinearShot(shot::SHOT_TYPE type, uint owner, float pos_x, float pos_y,
								 float direction_vector_x, float direction_vector_y,
								 uint dealing_damage, uint parameter);

	std::list< TERRAIN_TYPE > _currentlevel;
	MAYFACE_STORAGE _maybe_face; // список юнитов и стен с которыми могут столкнуться

	// functions
	bool ShutDown();
	bool Start();
	uint generateID();
	bool IsInRectangle(const float& x, const float& y, const float& x_c, const float& y_c, const float& a, const float& b);
public:
	friend class GL_Painter;
	Server();
	~Server();

	void UpdateAllObjects();

	void ConnectPainter();
	// (key, player_id)
	bool InputCommand(unsigned char, unsigned char);

	void PlayerClick(uint playerid, float x, float y);

	void ReturnForRender();

	void AddEnemy(float x, float y, UNIT_TYPE type);

	bool IsServerWork() const
	{
		return _IsServerWork;
	}

	void LoadLevel(const char*);
};

#endif // SERVER_H