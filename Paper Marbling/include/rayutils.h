#pragma once

#include <raymath.h>
#include <raylib.h>
#include <rlgl.h>

#define TWO_PI PI * 2

//----------------------------------------------------------------------------------
// Module Functions Definition - Color util
//----------------------------------------------------------------------------------

#define DEFAULTBG Color{53, 53, 53, 255}

//Generates a grayscale color
Color fill(float value)
{
	value = Clamp(value, 0.0f, 255.0f);

	auto grayscale = static_cast<unsigned char>(value);

	return Color{grayscale, grayscale, grayscale, 255};
}

// Generates a random color
Color GetRandomColor()
{
	auto r = static_cast<unsigned char>(GetRandomValue(0, 255));
	auto g = static_cast<unsigned char>(GetRandomValue(0, 255));
	auto b = static_cast<unsigned char>(GetRandomValue(0, 255));
	unsigned char a = 255;

	return Color{r, g, b, a};
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Vector2 utils
//----------------------------------------------------------------------------------

//Creates a 2D vector from a given angle
Vector2 Vector2FromAngle(float angle)
{
	return Vector2{cosf(angle), sinf(angle)};
}

//Generates a random 2D direction vector
Vector2 Vector2RandomDirection()
{
	float angle = GetRandomValue(0, 360) * (PI / 180);
	return Vector2FromAngle(angle);
}

//Limits the magnitude of a vector to the specified maximum length
Vector2 Vector2Limit(Vector2 vec, float max)
{
	if (!(Vector2LengthSqr(vec) > max * max)) return vec;

	return Vector2Scale(Vector2Normalize(vec), max);
}

//Computes the angle(heading) of a 2D vector
float Vector2Heading(Vector2 v)
{
	return atan2f(v.y, v.x);
}

//Sets the magnitude of a vector to the specified length
Vector2 Vector2SetMag(Vector2 vec, float len)
{
	return Vector2Scale(Vector2Normalize(vec), len);
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Drawing util
//----------------------------------------------------------------------------------

//Polygon draw with custom points
void DrawPolyPoint(Vector2 center, Vector2* points, int pointCount, Color color)
{
	if (pointCount >= 3 && points != NULL)
	{
		rlBegin(RL_TRIANGLES);
		rlColor4ub(color.r, color.g, color.b, color.a);
		for (int i = 0; i < pointCount; i++)
		{
			int j = i + 1;
			if (j == pointCount) j = 0;

			rlVertex2f(points[i].x, points[i].y);
			rlVertex2f(center.x, center.y);
			rlVertex2f(points[j].x, points[j].y);
		}

		rlEnd();
	}
}

//Polygon lines draw with custom points
void DrawPolyLinesPoint(Vector2* points, int pointCount, Color color)
{
	if (pointCount >= 3 && points != NULL)
	{
		rlBegin(RL_LINES);
		rlColor4ub(color.r, color.g, color.b, color.a);

		for (int i = 0; i < pointCount - 1; i++)
		{
			rlVertex2f(points[i].x, points[i].y);
			rlVertex2f(points[i + 1].x, points[i + 1].y);
		}

		rlVertex2f(points[pointCount - 1].x, points[pointCount - 1].y);
		rlVertex2f(points[0].x, points[0].y);

		rlEnd();
	}
}