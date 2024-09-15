#include <deque>
#include <iostream>
#include "raylib.h"
#include "raymath.h"

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int cellSize = 30;
int cellCount = 20;
int offset = 75;

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
	double currTime = GetTime();

	if (currTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currTime;
		return true;
	}
	return false;
}

bool ElementInDequeue(Vector2 element, std::deque<Vector2> dequeue) 
{
	for (const auto& i : dequeue) 
	{
		if (Vector2Equals(i, element))
			return true;
	}
	return false;
}

class Food
{
private:
	Texture2D texture;

public:
	Vector2 position;

	Food(std::deque<Vector2> snakeBody)
	{
		Image image = LoadImage("Resources/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(snakeBody);
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
	}

	Vector2 GenerateRandomCell()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y };
	}

	Vector2 GenerateRandomPos(std::deque<Vector2> snakeBody)
	{
		Vector2 position = GenerateRandomCell();

		do {
			position = GenerateRandomCell();
			std::cout << "food position : " << position.x << " " << position.y << std::endl;
		} while (ElementInDequeue(position, snakeBody) || position.x >= cellCount || position.y >= cellCount);

		return position;
	}
};

class Snake
{
private:

public:
	std::deque<Vector2> body = { Vector2{ 6, 9 }, Vector2{ 5, 9 }, Vector2{ 4, 9 } };
	Vector2 dir = { 1, 0 };
	bool addsegment = false;

	Snake() {}
	~Snake() {}

	void Draw()
	{
		Rectangle segment;
		for (const auto& i : body)
		{
			segment = Rectangle{ (float)offset + i.x * cellSize, (float)offset + i.y * cellSize, (float)cellSize, (float)cellSize };
			DrawRectangleRounded(segment, 0.5, 10, darkGreen);
		}
	}

	void Update()
	{
		body.push_front(Vector2Add(body[0], dir));

		if (addsegment)
			addsegment = false;

		else
			body.pop_back();
	}

	void Reset()
	{
		body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
		dir = { 1, 0 };
	}
};

class Game
{
private:

	Snake snake = Snake();
	Food food = Food(snake.body);
	bool running = true;
	int score = 0;

public:

	Game(){}
	~Game(){}

	void Draw()
	{
		BeginDrawing();
		{
			ClearBackground(green);
			DrawRectangleLinesEx(Rectangle{ (float)offset - 5,
				(float)offset - 5, 
				(float)cellCount * cellSize + 10, 
				(float)cellCount * cellSize + 10 }, 
				5, 
				darkGreen);
			DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);
			DrawText(TextFormat("%i", score), offset - 5, offset + cellSize * cellCount + 10, 40, darkGreen);
			food.Draw();
			snake.Draw();
		}
		EndDrawing();
	}

	void Update()
	{
		if (running)
		{
			snake.Update();
			CheckCollisionsWithFood();
			CheckColisionsWithEdges();
			CheckColisionsWithBody();
		}
	}

	void CheckCollisionsWithFood()
	{
		if (Vector2Equals(snake.body[0], food.position)) {
			food.position = food.GenerateRandomPos(snake.body);
			snake.addsegment = true;
			score++;
		}
	}

	void CheckColisionsWithEdges()
	{
		if (snake.body[0].x == cellCount || snake.body[0].x == -1)
			GameOver();

		if (snake.body[0].y == cellCount || snake.body[0].y == -1)
			GameOver();
	}

	void CheckColisionsWithBody()
	{
		std::deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();

		if (ElementInDequeue(snake.body[0], headlessBody))
			GameOver();
	}

	void GameOver()
	{
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.body);
		running = false;
		score = 0;
	}

	void HandleInput()
	{
		if (IsKeyPressed(KEY_UP) && snake.dir.y != 1)
		{
			snake.dir = { 0, -1 };
			running = true;
		}
		if (IsKeyPressed(KEY_DOWN) && snake.dir.y != -1)
		{
			snake.dir = { 0, 1 };
			running = true;
		}
		if (IsKeyPressed(KEY_LEFT) && snake.dir.x != 1)
		{
			snake.dir = { -1, 0 };
			running = true;
		}
		if (IsKeyPressed(KEY_RIGHT) && snake.dir.x != -1)
		{
			snake.dir = { 1, 0 };
			running = true;
		}
	}
};

int main()
{
	int resolution = 2 * offset + cellCount * cellSize;
	InitWindow(resolution, resolution, "Retro Snake");
	Image icon = LoadImage("Resources/LogoSbake.ico");
	SetWindowIcon(icon);

	SetTargetFPS(60);
	
	Game game = Game();

	while (!WindowShouldClose())
	{
		game.HandleInput();

		if (EventTriggered(0.2)) game.Update();

		game.Draw();
	}

	CloseAudioDevice();
	return 0;
}