#include "Game.h"
#include "OpenSimplexNoise.h"

class MarchingSq : public Game
{
private:
    std::vector<std::vector<float>> field;
    int rez = 5;
    int cols;
    int rows;
    float thresh = 0.1f;
    float incr = 0.05f;
    float zoff = 0.0f;
    OSN::Noise<3> noise;


public:
    MarchingSq() : Game("Marching Squares", 1200, 900)
    {
    }

    ~MarchingSq() {}

private:
    void OnCreate() override
    {
        cols = 1 + appWidth / rez;
        rows = 1 + appHeight / rez;

        field.resize(cols, std::vector<float>(rows, 0.0f));

    }

    void Update(float dt) override
    {
        float xoff = 0.0f;
        for (size_t i = 0; i < cols; i++)
        {
            xoff += incr;
            float yoff = 0.0f;
            for (size_t j = 0; j < rows; j++)
            {
                field[i][j] = noise.eval(xoff, yoff, zoff);
                yoff += incr;
            }

        }
        zoff += 0.01f;


    }

    void Render() override
    {
        DrawField();

        DrawMarchingSquares();

    }

    void DrawField()
    {
        for (size_t i = 0; i < cols; i++)
        {
            for (size_t j = 0; j < rows; j++)
            {
                auto remapped = Remap(field[i][j], -1, 1, -0.4f, 0.9f);

                DrawRectangle(
                    i * rez,
                    j * rez,
                    rez,
                    rez,
                    fill(remapped, DARKBLUE, RED)
                );

            }

        }
    }

    void DrawMarchingSquares()
    {
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
                int state = GetState(
                    field[i][j] > thresh ? 1 : 0,
                    field[i + 1][j] > thresh ? 1 : 0,
                    field[i + 1][j + 1] > thresh ? 1 : 0,
                    field[i][j + 1] > thresh ? 1 : 0
                );
                DrawMarch(state, c, d, b, a);

            }

        }
    }

    void DrawMarch(int state, const Vector2& c, const Vector2& d, const Vector2& b, const Vector2& a)
    {
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

    int GetState(int a, int b, int c, int d)
    {
        return (a << 3) | (b << 2) | (c << 1) | d;
    }
};
