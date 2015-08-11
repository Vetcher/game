#include "../headers/resource.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <cmath>

void drawCircleFill(float x, float y, float r, uint amountSegments, uint red, uint green, uint blue)
{
	
	glBegin(GL_POLYGON);
	glColor3f((float)red / 0xff, (float)green / 0xff, (float)blue / 0xff);
	for (uint i = 0; i < amountSegments; i++)
	{
		float angle = 2.0f * PI_f * float(i) / float(amountSegments);
		float dx = r * cosf(angle);
		float dy = r * sinf(angle);
		glVertex3f(x + dx, y + dy, 0.0f);
	}
	glEnd();
}

void drawCircle(float x, float y, float r, uint amountSegments, Color c)
{
	drawCircle(x, y, r, amountSegments, c.red(), c.green(), c.blue());
}

void drawCircleFill(float x, float y, float r, uint amountSegments, Color c)
{
	drawCircleFill(x, y, r, amountSegments, c.red(), c.green(), c.blue());
}

void drawCircle(float x, float y, float r, uint amountSegments, uint red, uint green, uint blue)
{
	
	glBegin(GL_LINE_LOOP);
	glColor3f((float)red / 0xff, (float)green / 0xff, (float)blue / 0xff);
	for (uint i = 0; i < amountSegments; i++)
	{
		float angle = 2.0f * PI_f * float(i) / float(amountSegments);
		float dx = r * cosf(angle);
		float dy = r * sinf(angle);
		glVertex3f(x + dx, y + dy, 0.0f);
	}
	glEnd();
}

float LENGTH(const float x, const float y)
{
	return sqrtf(x*x + y*y);
}

float ANGLE(const float x, const float y)
{
	const float sin = x / LENGTH(x, y);
	const float cos = y / LENGTH(x, y);
	if (cos <= 0)
		return asinf(-sin) / PI_f * 180.0f;
	else
		return (asinf(sin) - PI_f) / PI_f * 180.0f;
}

float LENGTH(const float x1, const float y1, const float x2, const float y2)
{
	const float a = x1 - x2;
	const float b = y1 - y2;
	return sqrtf(a*a + b*b);
}