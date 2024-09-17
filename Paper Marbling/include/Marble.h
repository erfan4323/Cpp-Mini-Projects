#pragma once
#include "Core/Game.h"
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
	void OnCreate() override
	{
		RegisterMouseAction(
			MOUSE_BUTTON_LEFT,
			InputState::Down,
			[this](const Mouse& mouse) { DropInk(mouse); }
		);
	}

	void OnDestroy() override
	{}

	void Update(float dt) override
	{
		auto x = GetRandomValue(0, width);
		auto y = GetRandomValue(0, height);
		auto r = GetRandomValue(10, 50);
		DropInk(x, y, r);
	}

	void Render() override
	{
		for (auto& drop : drops)
			drop.Draw();
	}

	void DropInk(const Mouse& mouse)
	{
		using namespace std::this_thread;
		using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
		using std::chrono::system_clock;

		DropInk(mouse.position.x, mouse.position.y, 50);

		sleep_for(50ms);
	}

	void DropInk(int x, int y, int r)
	{
		auto drop = Drop(x, y, r);

		for (auto& other : drops)
			other.Marble(drop);

		drops.push_back(drop);
	}
};
