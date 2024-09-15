#pragma once
#include <functional>
#include <unordered_map>
#include <raylib.h>

class InputManager
{
public:
	using Action = std::function<void()>;

private:
	std::unordered_map<int, Action> keyActions;
	std::unordered_map<int, Action> mouseActions;

public:
	void RegisterKeyAction(int key, Action action)
	{
		keyActions[key] = action;
	}

	void RegisterMouseAction(int button, Action action)
	{
		mouseActions[button] = action;
	}

	void ProcessInput() const
	{
		for (const auto& [key, action] : keyActions)
			if (IsKeyPressed(key) && action)
				action();

		for (const auto& [button, action] : mouseActions)
			if (IsMouseButtonDown(button) && action)
				action();
	}

	void Clear()
	{
		keyActions.clear();
		mouseActions.clear();
	}
};