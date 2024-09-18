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
			MouseLambda{DropInk(mouse);}
		);

		for (int i = 0; i < 50; i++)
			DropInk(400 + i, 400, 50);

		RegisterMouseAction(
			MOUSE_BUTTON_RIGHT,
			InputState::Down,
			MouseLambda{TineLine(mouse.position.x, 5, 16);}
		);
	}

	void OnDestroy() override
	{}

	void Update(float dt) override
	{}

	void Render() override
	{
		for (auto& drop : drops)
			drop.Draw();
	}

	void RandomDrop()
	{
		auto x = GetRandomValue(0, width);
		auto y = GetRandomValue(0, height);
		auto r = GetRandomValue(10, 50);
		DropInk(x, y, r);
	}

	void DropInk(const Mouse& mouse)
	{
		DropInk(
			mouse.position.x,
			mouse.position.y,
			mouse.velocity + 10
		);
	}

	void DropInk(int x, int y, int r)
	{
		auto drop = Drop(x, y, r);

		for (auto& other : drops)
			other.Marble(drop);

		drops.push_back(drop);
	}

	void TineLine(float xl, float z, float c)
	{
		for (auto& other : drops)
			other.Tine(xl, z, c);
	}
};
