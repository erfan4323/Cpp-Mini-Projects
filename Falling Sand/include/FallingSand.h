#include "Game.h"
#include "matrix.h"
#include <cmath>

class FallingSand : public Game
{
private:
	static constexpr int unit = 3;
	int width;
	int height;
	int cols;
	int rows;
	float hueValue = 0.0f;
	matrix<int> grid;
	matrix<int> nexGrid;
	Color boxColor = Color{193, 193, 193, 255};

public:
	FallingSand(std::string windowName, int width, int height) :
		Game(windowName.c_str(), width, height),
		width(width),
		height(height),
		cols(width / unit),
		rows(height / unit),
		grid(rows, cols, 0),
		nexGrid(rows, cols, 0)
	{}

	void OnCreate() override
	{
		RegisterMouseAction(MOUSE_BUTTON_LEFT, [this]() { SpawnSand(); });
	}

	void Update(float dt) override
	{
		nexGrid.SetAllTo(0);

		grid.iter([&](int& value, int r, int c)
		{
			auto state = grid(r, c);
			if (state > 0)
			{
				auto below = state.index(DOWN);
				Direction randomDir = GetRandomValue(0, 1) == 0 ? RIGHT_DOWN : LEFT_DOWN;

				auto belowRandom = state.index(randomDir);

				if (below == 0 && r < rows - 1)
				{
					nexGrid(r, c) = 0;
					nexGrid(r + 1, c) = state.get();
				}
				else if (IsSameDirection(randomDir, RIGHT_DOWN, belowRandom))
				{
					nexGrid(r, c + 1) = state.get();
				}
				else if (IsSameDirection(randomDir, LEFT_DOWN, belowRandom))
				{
					nexGrid(r, c - 1) = state.get();
				}
				else
				{
					nexGrid(r, c) = state.get();
				}
			}
		});

		grid = nexGrid;
	}

	void Render() override
	{
		grid.iter([&](int& value, int r, int c)
		{
			boxColor = ColorFromHSV(255, 255, static_cast<float>(value));
			float xPos = c * unit;
			float yPos = r * unit;
			if (grid(r, c) > 0)
				DrawRectangle(xPos, yPos, unit, unit, boxColor);
			//else
			//	DrawRectangleLines(xPos, yPos, unit, unit, boxColor);
		});
	}

	void OnDestroy() override
	{}

private:
	void SpawnSand()
	{
		auto mousePos = GetMousePosition();
		int mouseCol = std::floor(mousePos.x / unit);
		int mousrRow = std::floor(mousePos.y / unit);

		int matx = 20;
		int extent = std::floor(matx / 2);
		for (int i = -extent; i <= extent; i++)
		{
			for (int j = -extent; j <= extent; j++)
			{
				auto rnd = GetRandomValue(0, 100);
				if (rnd < 40)
				{
					int col = mouseCol + i;
					int row = mousrRow + j;
					if (WithinBounds(row, col))
						grid(row, col) = hueValue;
				}
			}
		}
		hueValue += 1.5f;
		if (hueValue >= 255.0f) hueValue = 0.0f;
	}

	bool IsSameDirection(Direction randomDir, Direction indentDir, int belowRandom)
	{
		return randomDir == indentDir && belowRandom == 0;
	}

	bool WithinBounds(int row, int col) const
	{
		return col >= 0 && col <= cols - 1 && row >= 0 && row <= rows - 1;
	}
};
