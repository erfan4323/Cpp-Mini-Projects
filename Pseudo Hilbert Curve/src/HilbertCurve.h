#include "Game.h"
#include <iostream>

class HilbertCurve : public Game
{
private:
    int order = 7;
    int n = pow(2, order);
    int total = n * n;
    std::vector<Vector2> path;
    int counter = 0;
    float animationSpeed = 50.0f; // Controls the speed of the animation (increase for faster)
    float timeElapsed = 0.0f;

public:
    HilbertCurve(/* args */) : Game("Hilbert Curve", 1024, 1024) {}
    ~HilbertCurve() {}

private:
    void OnCreate() override
    {
        path.resize(total);

        float len = appWidth / n;  // Calculate length once and apply transformations to the path
        for (size_t i = 0; i < total; i++)
        {
            path[i] = Hilbert(i);
            path[i] = Vector2Scale(path[i], len);
            path[i] = Vector2AddValue(path[i], len / 2);
        }

    }

    void Update(float dt) override
    {

        // Update the time elapsed based on frame time
        timeElapsed += dt * animationSpeed;

        // Only increase the counter if enough time has passed to create an animation effect
        if (timeElapsed >= 1.0f)
        {
            counter += 1000;
            timeElapsed = 0.0f;  // Reset time counter

            // Stop the counter at the total number of points to prevent it from resetting
            if (counter >= total)
            {
                counter = 0;
            }
        }
    }

    void Render() override
    {
        for (size_t i = 0; i < counter; i++)
        {
            DrawLineV(path[i], path[i + 1], RAYWHITE);
        }
    }

    Vector2 Hilbert(int i)
    {
        std::vector<Vector2> points{
            {0, 0},
            {0, 1},
            {1, 1},
            {1, 0}
        };

        int index = i & 3;
        Vector2 v = points[index];

        for (size_t j = 1; j < order; j++)
        {
            i = i >> 2;
            index = i & 3;

            float len = pow(2, j);

            switch (index)
            {
            case 0:
                v = { v.y, v.x };
                break;

            case 1:
                v.y += len;
                break;

            case 2:
                v.x += len;
                v.y += len;
                break;

            case 3:
                v = { len - 1 - v.y, len - 1 - v.x };
                v.x += len;
                break;

            default:
                break;
            }
        }
        return v;
    }

    void Debug(const std::vector<Vector2>& path, size_t i)
    {
        DrawCircle(path[i].x, path[i].y, 5, RED);
        std::string pointNumber = std::to_string(i);
        DrawText(pointNumber.c_str(), path[i].x + 10, path[i].y - 10, 10, RAYWHITE);
    }
};