#pragma once
#include <functional>
#include <raylib.h>
#include <unordered_map>

enum class InputState
{
	Pressed,
	Down,
	Release,
	Up
};

class InputManager
{
public:
	using Action = std::function<void()>;

private:
	struct ActionInfo
	{
		Action action;
		InputState state;
	};

	std::unordered_map<int, ActionInfo> keyActions;
	std::unordered_map<int, ActionInfo> mouseActions;

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
	void RegisterKeyAction(int key, Action action, InputState mouseState)
	{
		keyActions[key] = {action, mouseState};
	}

	void RegisterMouseAction(int button, Action action, InputState mouseState)
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
				action();
		}
	}

	void Clear()
	{
		keyActions.clear();
		mouseActions.clear();
	}
};
