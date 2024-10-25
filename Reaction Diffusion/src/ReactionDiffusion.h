#include "Game.h"
#include "matrix.h"
#include <memory>

class ReactionDiffusion : public Game
{
private:
    std::vector<std::vector<Vector2>> grid;
    std::vector<std::vector<Vector2>> nextGrid;

    Texture2D texture;
    std::unique_ptr<Color[]> pixels;

    float da = 1.0f;
    float db = 0.5f;
    float feed = 0.055f;
    float k = 0.062f;

public:
    ReactionDiffusion()
        : Game("Reaction Diffusion Algorithm", 1200, 800),
        grid(appHeight, std::vector<Vector2>(appWidth, { 1.0f, 0 })),
        nextGrid(appHeight, std::vector<Vector2>(appWidth, { 1.0f, 0 })),
        pixels(std::make_unique<Color[]>(appWidth* appHeight))
    {
    }

    ~ReactionDiffusion() {}

private:
    void OnCreate() override
    {
        texture = LoadTextureFromImage(GenImageColor(appWidth, appHeight, RAYWHITE));

        for (size_t x = 0; x < appWidth; x++)
        {
            for (size_t y = 0; y < appHeight; y++)
            {
                size_t pix = x + y * appWidth;
                pixels[pix] = WHITE;
            }
        }

        for (size_t x = 100; x < 110; x++)
        {
            for (size_t y = 100; y < 110; y++)
            {
                grid[y][x].y = 1;
                grid[y][x].x = 0.5f;
            }
        }

        UpdateTexture(texture, pixels.get());
    }

    void Update(float dt) override
    {
        for (size_t x = 1; x < appWidth - 1; x++)
        {
            for (size_t y = 1; y < appHeight - 1; y++)
            {
                auto a = grid[y][x].x;
                auto b = grid[y][x].y;
                nextGrid[y][x].x = a + (da * LaplaceA(x, y)) - (a * b * b) + (feed * (1 - a));
                nextGrid[y][x].y = b + (db * LaplaceB(x, y)) + (a * b * b) - ((k + feed) * b);

                nextGrid[y][x].x = Clamp(nextGrid[y][x].x, 0.0f, 1.0f);
                nextGrid[y][x].y = Clamp(nextGrid[y][x].y, 0.0f, 1.0f);
            }
        }
    }

    void Render() override
    {
        for (size_t x = 0; x < appWidth; x++)
        {
            for (size_t y = 0; y < appHeight; y++)
            {
                size_t pix = x + y * appWidth;

                auto red = SetPixelColor(nextGrid[y][x].x);
                auto blue = SetPixelColor(nextGrid[y][x].y);
                auto a = grid[y][x].x;
                auto b = grid[y][x].y;
                auto c = floor((a - b) * 255);
                c = Clamp(c, 0, 255);
                auto r = SetPixelColor(c);

                pixels[pix] = { r, r, r, 255 };
            }
        }
        UpdateTexture(texture, pixels.get());
        DrawTexture(texture, 0, 0, WHITE);
        std::swap(grid, nextGrid);
    }

    void OnDestroy() override
    {
        UnloadTexture(texture);
    }

    float LaplaceA(int x, int y)
    {
        float sumA = 0.0f;

        // Calculate Laplace for A
        sumA += grid[y][x].x * -1.0f;
        sumA += grid[y - 1][x].x * 0.2f;  // Up
        sumA += grid[y + 1][x].x * 0.2f;  // Down
        sumA += grid[y][x - 1].x * 0.2f;  // Left
        sumA += grid[y][x + 1].x * 0.2f;  // Right
        sumA += grid[y - 1][x - 1].x * 0.05f; // Top Left
        sumA += grid[y + 1][x + 1].x * 0.05f; // Bottom Right
        sumA += grid[y + 1][x - 1].x * 0.05f; // Bottom Left
        sumA += grid[y - 1][x + 1].x * 0.05f; // Top Right

        return sumA; // Return the result
    }

    float LaplaceB(int x, int y)
    {
        float sumB = 0.0f;

        // Calculate Laplace for B
        sumB += grid[y][x].y * -1.0f;
        sumB += grid[y - 1][x].y * 0.2f;  // Up
        sumB += grid[y + 1][x].y * 0.2f;  // Down
        sumB += grid[y][x - 1].y * 0.2f;  // Left
        sumB += grid[y][x + 1].y * 0.2f;  // Right
        sumB += grid[y - 1][x - 1].y * 0.05f; // Top Left
        sumB += grid[y + 1][x + 1].y * 0.05f; // Bottom Right
        sumB += grid[y + 1][x - 1].y * 0.05f; // Bottom Left
        sumB += grid[y - 1][x + 1].y * 0.05f; // Top Right

        return sumB; // Return the result
    }
};
