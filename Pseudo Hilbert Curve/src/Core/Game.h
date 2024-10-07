#pragma once
#include <string>
#include <functional>
#include <raylib.h>
#include "InputManager.h"
#include "ImGuiInitializer.h"

class Game
{
private: // Properties
	ImGuiIO io;
	InputManager inputManager;

	bool backgroundCleared = false;

protected: // Window Options
	std::string appName = "Un initialized";
	int appWidth = 500;
	int appHeight = 500;
	ConfigFlags appWindowFlag = FLAG_WINDOW_RESIZABLE;


public: // Constructors & Destructors
	Game(std::string windowName,
		int width,
		int height,
		ConfigFlags windowFlag = FLAG_WINDOW_RESIZABLE) :
		appWidth(width),
		appHeight(height),
		appName(windowName),
		appWindowFlag(windowFlag)
	{
		SetConfigFlags(appWindowFlag);
		InitWindow(width, height, windowName.c_str());
		SetTargetFPS(60);

		ImGuiInitialize(io);
	}

	~Game()
	{
		ImGuiUnInitialize();

		CloseWindow();
	}

private: // Configs
	void ConfigRender(const std::function<void()>& render)
	{
		BeginDrawing();
		ClearBackground(Color{ 53, 53, 53, 255 });

		ImguiBeginDrawing();

		render();

		ImguiEndDrawing(io);

		EndDrawing();
	}

protected: // Client Side coding and helpers
	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void OnGUI() {}
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

	void RegisterKeyAction(int key, InputState inputState, InputManager::KeyAction action)
	{
		inputManager.RegisterKeyAction(key, action, inputState);
	}

	void RegisterMouseAction(int button, InputState inputState, InputManager::MouseAction action)
	{
		inputManager.RegisterMouseAction(button, action, inputState);
	}

public: // Code runner methods
	void GameLoop()
	{
		OnCreate();

		while (!WindowShouldClose())
		{
			float dt = GetFrameTime();

			inputManager.ProcessInput();

			Update(dt);

			ConfigRender([this]() { OnGUI(); Render(); });
		}

		OnDestroy();
	}
};
