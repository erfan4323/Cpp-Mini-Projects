#pragma once
#include <string>
#include <functional>
#include <raylib.h>
#include "InputManager.h"

#pragma region imgui
#include "imgui.h"
#include "rlImGui.h"
#include "imguiThemes.h"
#include <InputManager.h>
#pragma endregion

class Game
{
private:
	ImGuiIO io;
	InputManager inputManager;

public:
	Game(std::string windowName, int width, int height)
	{
		SetConfigFlags(FLAG_WINDOW_RESIZABLE);
		InitWindow(width, height, windowName.c_str());
		SetTargetFPS(60);

		#pragma region imgui
		rlImGuiSetup(true);

		//you can use whatever imgui theme you like!
		//ImGui::StyleColorsDark();
		//imguiThemes::yellow();
		imguiThemes::gray();
		//imguiThemes::green();
		//imguiThemes::red();
		//imguiThemes::embraceTheDarkness();

		ImGuiIO& io = ImGui::GetIO(); (void) io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.FontGlobalScale = 2;

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			//style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 0.5f;
			//style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
		}
		this->io = io;

		#pragma endregion
	}

	~Game()
	{
		#pragma region imgui
		rlImGuiShutdown();
		#pragma endregion

		CloseWindow();
	}

private:
	void ConfigRender(const std::function<void()>& render) const
	{
		BeginDrawing();
		ClearBackground(Color{53, 53, 53, 255});
		#pragma region imgui
		rlImGuiBegin();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
		#pragma endregion
		render();
		#pragma region imgui
		rlImGuiEnd();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		#pragma endregion
		EndDrawing();
	}

public:
	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void Update(float dt) {}
	virtual void Render() {}

	void RegisterKeyAction(int key, InputState inputState, InputManager::Action action)
	{
		inputManager.RegisterKeyAction(key, action, inputState);
	}

	void RegisterMouseAction(int button, InputState inputState, InputManager::Action action)
	{
		inputManager.RegisterMouseAction(button, action, inputState);
	}

	void GameLoop()
	{
		OnCreate();

		while (!WindowShouldClose())
		{
			float dt = GetFrameTime();

			inputManager.ProcessInput();

			Update(dt);

			ConfigRender([this]() { Render(); });
		}

		OnDestroy();
	}
};
