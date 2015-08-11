#include "../headers/shots.h"

BaseShot::BaseShot(float p_x, float p_y, float d_x, float d_y, int damage, uint own)
{
	x = p_x;
	y = p_y;
	//dir_x = d_x;
	//dir_y = d_y;
	dir_x = speed * d_x / LENGTH(d_x, d_y);
	dir_y = speed * d_y / LENGTH(d_x, d_y);
	dmg = damage;
	owner = own;
}

void BaseShot::DrawIt()
{
	drawCircleFill(x, y, radius, 50, 70, 70, 30);
}

void BaseShot::UpdateShot()
{
	if (distance <= 0) _isAlive = false;
	distance -= speed;
	x += dir_x;
	y += dir_y;
}