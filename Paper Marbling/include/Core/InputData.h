#pragma once

#include "Common/rayutils.h"

enum class InputState
{
	Pressed,
	Down,
	Release,
	Up
};

struct Mouse
{
	Vector2 delta;
	Vector2 position;
	float wheelMove;
	Vector2 wheelMoveV;
	float velocity;
	float angle;

	Mouse() :
		delta(GetMouseDelta()),
		position(GetMousePosition()),
		wheelMove(GetMouseWheelMove()),
		wheelMoveV(GetMouseWheelMoveV()),
		velocity(Vector2Length(delta)),
		angle(Vector2Heading(delta))
	{}
};