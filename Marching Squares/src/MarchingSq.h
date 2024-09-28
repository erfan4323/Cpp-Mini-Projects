#include "Game.h"
#include "PerlinNoise.h"

class MarchingSq : public Game
{
private:
    std::vector<std::vector<float>> field;
    int rez = 20;
    int cols;
    int rows;
    PerlinNoise noise;


public:
    MarchingSq() : Game("Marching Squares", 800, 600)
    {
    }

    ~MarchingSq() {}

private:
    void OnCreate() override
    {
        cols = 1 + appWidth / rez;
        rows = 1 + appHeight / rez;

        field.resize(cols, std::vector<float>(rows, 0.0f));
        float xoff = 0.f;
        for (size_t i = 0; i < cols; i++)
        {
            float yoff = 0.0f;
            for (size_t j = 0; j < rows; j++)
            {
                field[i][j] = noise.noise(xoff, yoff);
            }

        }
    }

    void Update(float dt) override
    {

    }

    void Render() override
    {
        for (size_t i = 0; i < cols; i++)
        {
            for (size_t j = 0; j < rows; j++)
            {
                DrawCircle(i * rez,
                    j * rez,
                    rez * 0.2,
                    fill(Remap(field[i][j], 0, 1, 0, 255)));
            }

        }

        for (size_t i = 0; i < cols - 1; i++)
        {
            for (size_t j = 0; j < rows - 1; j++)
            {
                float x = i * rez;
                float y = j * rez;
                Vector2 a = { x + rez * 0.5, y };
                Vector2 b = { x + rez, y + rez * 0.5 };
                Vector2 c = { x + rez * 0.5, y + rez };
                Vector2 d = { x, y + rez * 0.5 };
                auto state = GetState(
                    field[i][j],
                    field[i + 1][j],
                    field[i + 1][j + 1],
                    field[i][j + 1]
                );
                switch (state)
                {
                case 1:
                    DrawLineV(c, d, RAYWHITE);
                    break;
                case 2:
                    DrawLineV(b, c, RAYWHITE);
                    break;
                case 3:
                    DrawLineV(b, d, RAYWHITE);
                    break;
                case 4:
                    DrawLineV(a, b, RAYWHITE);
                    break;
                case 5:
                    DrawLineV(a, d, RAYWHITE);
                    DrawLineV(b, c, RAYWHITE);
                    break;
                case 6:
                    DrawLineV(a, c, RAYWHITE);
                    break;
                case 7:
                    DrawLineV(a, d, RAYWHITE);
                    break;
                case 8:
                    DrawLineV(a, d, RAYWHITE);
                    break;
                case 9:
                    DrawLineV(a, c, RAYWHITE);
                    break;
                case 10:
                    DrawLineV(a, b, RAYWHITE);
                    DrawLineV(c, d, RAYWHITE);
                    break;
                case 11:
                    DrawLineV(a, b, RAYWHITE);
                    break;
                case 12:
                    DrawLineV(b, d, RAYWHITE);
                    break;
                case 13:
                    DrawLineV(b, c, RAYWHITE);
                    break;
                case 14:
                    DrawLineV(c, d, RAYWHITE);
                    break;
                default:
                    break;
                }
            }

        }
    }

    int GetState(int a, int b, int c, int d)
    {
        return a * 8 + b * 4 + c * 2 + d * 1;
    }
};