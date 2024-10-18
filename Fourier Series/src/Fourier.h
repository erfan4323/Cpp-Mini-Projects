#include "Game.h"

class Fourier : public Game
{
private:
    float time = 0.0f;

    std::vector<float> wave;
public:
    Fourier() : Game("Fourier Series", 800, 800) {}
    ~Fourier() {}

private:
    void Update(float dt) override
    {
        time += 0.03;

    }

    void Render() override
    {
        auto x = 0.0f;
        auto y = 0.0f;

        auto circlePos = Vector2{ 150, 200 };


        for (size_t i = 0; i < 5; i++)
        {
            int n = i * 2 + 1;
            float radius = 100.0f * (4.0f / (n * PI));
            auto prevx = x;
            auto prevy = y;
            x += radius * cos(n * time);
            y += radius * sin(n * time);

            DrawCircleLines(prevx + circlePos.x, prevy + circlePos.y, radius, { 245, 245, 245, 100 });
            DrawLineV({ prevx + circlePos.x, prevy + circlePos.y }, { x + circlePos.x, y + circlePos.y }, RAYWHITE);


        }
        wave.insert(wave.begin(), y);
        DrawLine(
            x + circlePos.x, y + circlePos.y,
            circlePos.x + 200, wave[0] + circlePos.y,
            PURPLE
        );

        if (wave.size() > 1)
            for (size_t i = 0; i < wave.size() - 1; i++)
                DrawLine(
                    i + circlePos.x + 200,
                    wave[i] + circlePos.y,
                    (i + 1) + circlePos.x + 200,
                    wave[i + 1] + circlePos.y,
                    YELLOW
                );

        if (wave.size() > 500)
            wave.pop_back();
    }
};