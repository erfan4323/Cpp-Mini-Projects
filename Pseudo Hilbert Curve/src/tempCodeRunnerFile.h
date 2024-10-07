#include "Game.h"
#include <iostream>

class HilbertCurve : public Game
{
private:
    int order = 1;
    int n = pow(2, order);
    int total = n * n;
    std::vector<Vector2> path;
    int counter = 0;

public:
    HilbertCurve(/* args */) : Game("Hilbert Curve", 1024, 1024) {}
    ~HilbertCurve() {}

private:
    void OnCreate() override
    {
        path.resize(total);

    }

    void Update(float dt) override
    {
        for (size_t i = 0; i < total; i++)
        {
            path[i] = Hilbert(i);
            float len = appWidth / n;
            path[i] = Vector2Scale(path[i], len);
            path[i] = Vector2AddValue(path[i], len / 2);
        }

    }
    void Render() override
    {
        for (size_t i = 0; i < path.size(); i++)
        {
            if (path[i + 1] == nullptr) break;
            DrawLineV(path[i], path[i + 1], RAYWHITE);
            DrawCircle(path[i].x, path[i].y, 10, RED);
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

        return points[i];
    }
};