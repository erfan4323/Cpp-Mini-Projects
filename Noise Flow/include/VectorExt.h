#pragma once

#include <raymath.h>

#define TWO_PI PI * 2

Color fill(float value)
{
	// Clamp the value between 0 and 1
	if (value < 0.0f) value = 0.0f;
	if (value > 1.0f) value = 1.0f;

	// Multiply value by 255 to get the grayscale color
	unsigned char grayscale = (unsigned char) (value * 255.0f);

	// Return a grayscale color (R, G, B are the same)
	return Color{grayscale, grayscale, grayscale, 255}; // 255 is the alpha (opaque)
}

Vector2 FromAngle(float angle, Vector2* target = nullptr)
{
	float x = cosf(angle);
	float y = sinf(angle);

	if (target == nullptr)
	{
		return Vector2{x, y};
	}
	else
	{
		target->x = x;
		target->y = y;
		return *target;
	}
}

Vector2 GenerateRandomDirection()
{
	float angle = static_cast<float>(std::rand()) / RAND_MAX * TWO_PI;
	return FromAngle(angle);
}

Vector2 Vector2Limit(Vector2 vec, float max)
{
	if (Vector2LengthSqr(vec) > max * max)
	{
		vec = Vector2Normalize(vec);
		vec = Vector2Scale(vec, max);
	}
	return vec;
}

float Vector2Heading(Vector2 v)
{
	return atan2f(v.y, v.x);
}

Vector2 Vector2SetMag(Vector2 vec, float len)
{
	vec = Vector2Normalize(vec);
	vec = Vector2Scale(vec, len);
	return vec;
}