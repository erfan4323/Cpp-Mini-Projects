#pragma once
#include <functional>
#include "raylib.h"
#include "Common/rayutils.h"
#include <unordered_map>

#define MouseLambda [this](const Mouse& mouse)

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

class InputManager
{
public:
	using KeyAction = std::function<void()>;
	using MouseAction = std::function<void(const Mouse&)>;

private:
	struct KeyActionInfo
	{
		KeyAction action;
		InputState state;
	};

	struct MouseActionInfo
	{
		MouseAction action;
		InputState state;
	};

	std::unordered_map<int, KeyActionInfo> keyActions;
	std::unordered_map<int, MouseActionInfo> mouseActions;

	bool IsKeyInState(int key, InputState state) const
	{
		switch (state)
		{
		case InputState::Pressed: return IsKeyPressed(key);
		case InputState::Down: return IsKeyDown(key);
		case InputState::Release: return IsKeyReleased(key);
		case InputState::Up: return IsKeyUp(key);
		default: return false;
		}
	}

	bool IsMouseButtonInState(int button, InputState state) const
	{
		switch (state)
		{
		case InputState::Pressed: return IsMouseButtonPressed(button);
		case InputState::Down: return IsMouseButtonDown(button);
		case InputState::Release: return IsMouseButtonReleased(button);
		case InputState::Up: return IsMouseButtonUp(button);
		default: return false;
		}
	}

public:
	void RegisterKeyAction(int key, KeyAction action, InputState mouseState)
	{
		keyActions[key] = {action, mouseState};
	}

	void RegisterMouseAction(int button, MouseAction action, InputState mouseState)
	{
		mouseActions[button] = {action, mouseState};
	}

	void ProcessInput()
	{
		for (const auto& [key, actionInfo] : keyActions)
		{
			const auto& [action, requiredState] = actionInfo;
			if (IsKeyInState(key, requiredState))
				action();
		}

		for (const auto& [button, actionInfo] : mouseActions)
		{
			const auto& [action, requiredState] = actionInfo;
			if (IsMouseButtonInState(button, requiredState))
			{
				Mouse currentMouse;
				action(currentMouse);
			}
		}
	}

	void Clear()
	{
		keyActions.clear();
		mouseActions.clear();
	}
};
