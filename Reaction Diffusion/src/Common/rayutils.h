#pragma once

#include <raymath.h>
#include <raylib.h>
#include <rlgl.h>

#define TWO_PI PI * 2
#define HALF_PI PI / 2

//----------------------------------------------------------------------------------
// Module Functions Definition - Color util
//----------------------------------------------------------------------------------

#define DEFAULTBG Color{53, 53, 53, 255}
#define PINKBLUEGRAD Vector3{100, 180, 230}

//Generates a grayscale color
Color fill(float value)
{
	value = Clamp(value, 0.0f, 255.0f);

	auto grayscale = static_cast<unsigned char>(value);

	return Color{ grayscale, grayscale, grayscale, 255 };
}

// Generates a smooth gradient color between two colors
Color fill(float value, Color color1, Color color2)
{
	value = Clamp(value, 0.0f, 1.0f);

	auto r = static_cast<unsigned char>(color1.r + (color2.r - color1.r) * value);
	auto g = static_cast<unsigned char>(color1.g + (color2.g - color1.g) * value);
	auto b = static_cast<unsigned char>(color1.b + (color2.b - color1.b) * value);
	auto a = static_cast<unsigned char>(color1.a + (color2.a - color1.a) * value);

	return Color{ r, g, b, a };
}

// Returns an unsigned char to fill Color datas
template<typename T>
unsigned char SetPixelColor(T value)
{
	return static_cast<unsigned char>(Clamp(value, 0.0f, 255.0f));
}

// Generates a random color
Color GetRandomColor(Vector3 seed = { 0 })
{
	auto r = static_cast<unsigned char>(GetRandomValue(seed.x, 255));
	auto g = static_cast<unsigned char>(GetRandomValue(seed.y, 255));
	auto b = static_cast<unsigned char>(GetRandomValue(seed.z, 255));
	unsigned char a = 255;

	return Color{ r, g, b, a };
}

Color GetHSVColor(int step, int total)
{
	float h = (float)step / total * 360.0f;

	return ColorFromHSV(h, 1.0f, 1.0f);
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Vector2 utils
//----------------------------------------------------------------------------------

//Creates a 2D vector from a given angle
Vector2 Vector2FromAngle(float angle)
{
	return Vector2{ cosf(angle), sinf(angle) };
}

//Gives an angle in 360 degrees from a Vector2 (NOTE: you can primarily use Vector2Heading)
float Vector2ToAngle(const Vector2& delta)
{
	float angleRad = atan2(-delta.y, delta.x);
	float angleDeg = angleRad * (180.0f / PI);

	if (angleDeg < 0.0f)
		angleDeg += 360.0f;

	return angleDeg;
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