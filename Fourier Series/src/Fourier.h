#include "Game.h"

class Fourier : public Game
{
private:
    float time = 0.0f;
    std::vector<float> wave;

    static constexpr size_t maxWaveSize = 700;      // Limit for wave size
    static constexpr int maxHarmonics = 20;         // Number of harmonics
    static constexpr float radiusScale = 100.0f;    // Scale for the radius
    static constexpr float speed = 0.03f;           // Speed of the animation
    static constexpr float waveOffsetX = 250.0f;    // Offset for the wave drawing

public:
    Fourier() : Game("Fourier Series", 1200, 800) {}
    ~Fourier() {}

private:
    void Update(float dt) override { time -= speed; }

    void Render() override
    {
        float x = 0.0f;
        float y = 0.0f;

        const Vector2 circlePos{ appWidth / 5, appHeight / 2 };

        for (size_t i = 0; i < maxHarmonics; ++i)
        {
            int n = i * 2 + 1;
            float radius = radiusScale * (4.0f / (n * PI));

            float prevX = x;
            float prevY = y;

            x += radius * cos(n * time);
            y += radius * sin(n * time);

            Vector2 centerPos = { prevX + circlePos.x, prevY + circlePos.y };
            Vector2 pointedPos = { x + circlePos.x, y + circlePos.y };

            DrawCircleLinesV(centerPos, radius, { 245, 245, 245, 100 });
            DrawLineV(centerPos, pointedPos, RAYWHITE);
        }

        wave.emplace(wave.begin(), y);

        DrawLine(
            x + circlePos.x, y + circlePos.y,
            circlePos.x + waveOffsetX, wave[0] + circlePos.y,
            PURPLE
        );

        if (wave.size() > 1)
            for (size_t i = 0; i < wave.size() - 1; ++i)
                DrawLine(
                    i + circlePos.x + waveOffsetX,
                    wave[i] + circlePos.y,
                    (i + 1) + circlePos.x + waveOffsetX,
                    wave[i + 1] + circlePos.y,
                    YELLOW
                );

        while (wave.size() > maxWaveSize)
            wave.erase(wave.end());
    }
};
