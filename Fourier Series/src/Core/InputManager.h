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
	void RegisterKeyAction(int key, KeyAction action, InputState keyState)
	{
		keyActions.insert({key, {action, keyState}});
	}

	void RegisterMouseAction(int button, MouseAction action, InputState mouseState)
	{
		mouseActions.insert({button, {action, mouseState}});
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
