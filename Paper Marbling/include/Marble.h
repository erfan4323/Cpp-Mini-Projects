#pragma once
#include "Game.h"
#include "Ink.h"
#include <chrono>
#include <thread>

class Marble : public Game
{
private:
	int width;
	int height;
	std::vector<Drop> drops;

public:
	Marble(std::string header, int width, int height) :
		Game(header, width, height),
		width(width), height(height)
	{}

	~Marble()
	{}

private:
	void OnCreate()
	{
		RegisterMouseAction(
			MOUSE_BUTTON_LEFT,
			InputState::Down,
			[this]() { DropInk(); }
		);
	}

	void OnDestroy()
	{}

	void Update(float dt) override
	{}

	void Render() override
	{
		auto x = GetRandomValue(0, width);
		auto y = GetRandomValue(0, height);
		auto r = GetRandomValue(10, 50);
		DropInk(x, y, r);

		for (auto& drop : drops)
			drop.Draw();
	}

	void DropInk()
	{
		using namespace std::this_thread;
		using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
		using std::chrono::system_clock;

		auto mouse = GetMousePosition();
		auto drop = Drop(mouse.x, mouse.y, 50);

		for (auto& other : drops)
			other.Marble(drop);

		sleep_for(50ms);

		drops.push_back(drop);
	}

	void DropInk(int x, int y, int r)
	{
		auto drop = Drop(x, y, r);

		for (auto& other : drops)
			other.Marble(drop);

		drops.push_back(drop);
	}
};
