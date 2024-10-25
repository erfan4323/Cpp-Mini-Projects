#pragma once
#include <functional>
#include <map>

#include "InputData.h"

#define MouseLambda [this](const Mouse& mouse)

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

	std::multimap<int, KeyActionInfo> keyActions;
	std::multimap<int, MouseActionInfo> mouseActions;

	Mouse currentMouse;

	bool IsKeyInState(int key, InputState state) const
	{
		switch (state)
		{
			using enum InputState;
		case Pressed: return IsKeyPressed(key);
		case Down: return IsKeyDown(key);
		case Release: return IsKeyReleased(key);
		case Up: return IsKeyUp(key);
		default: return false;
		}
	}

	bool IsMouseButtonInState(int button, InputState state) const
	{
		switch (state)
		{
			using enum InputState;
		case Pressed: return IsMouseButtonPressed(button);
		case Down: return IsMouseButtonDown(button);
		case Release: return IsMouseButtonReleased(button);
		case Up: return IsMouseButtonUp(button);
		default: return false;
		}
	}

public:
	void RegisterKeyAction(int key, KeyAction const& action, InputState keyState)
	{
		keyActions.insert({ key, {action, keyState} });
	}

	void RegisterMouseAction(int button, MouseAction const& action, InputState mouseState)
	{
		mouseActions.insert({ button, {action, mouseState} });
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
				currentMouse.Update();
				action(currentMouse);
			}
		}
	}

	void Flush()
	{
		keyActions.clear();
		mouseActions.clear();
	}
};
