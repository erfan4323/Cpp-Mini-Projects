#pragma once

#include <raymath.h>

#define TWO_PI PI * 2

//----------------------------------------------------------------------------------
// Module Functions Definition - Color util
//----------------------------------------------------------------------------------

//Generates a grayscale color 
Color Fill(float value)
{
	value = Clamp(value, 0.0f, 1.0f);

	unsigned char grayscale = (unsigned char) (value * 255.0f);

	return Color{grayscale, grayscale, grayscale, 255}; 
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