#include "Game.h"

class Fourier : public Game
{
private:
    float time = 0.0f;
    std::vector<float> wave;

    int maxHarmonics;       // Max harmonics 
    int radiusScale;      // Radius scale 
    float speed;            // Speed 

public:

    Fourier() : Game("Fourier Series", 1200, 800),
        maxHarmonics(20),
        radiusScale(100.0f),
        speed(0.03f)
    {
    }

    ~Fourier() {}

private:
    void Update(float dt) override
    {
        time -= speed;
    }

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

            DrawCircleLines(prevX + circlePos.x, prevY + circlePos.y, radius, { 245, 245, 245, 100 });
            DrawLineV({ prevX + circlePos.x, prevY + circlePos.y }, { x + circlePos.x, y + circlePos.y }, RAYWHITE);
        }

        wave.insert(wave.begin(), y);

        DrawLine(
            x + circlePos.x, y + circlePos.y,
            circlePos.x + 200, wave[0] + circlePos.y,
            PURPLE
        );

        if (wave.size() > 1)
        {
            for (size_t i = 0; i < wave.size() - 1; ++i)
            {
                DrawLine(
                    i + circlePos.x + 200,
                    wave[i] + circlePos.y,
                    (i + 1) + circlePos.x + 200,
                    wave[i + 1] + circlePos.y,
                    YELLOW
                );
            }
        }

        while (wave.size() > 1 && (wave.size() - 1 + circlePos.x + 200.0f) < 0)
        {
            wave.erase(wave.begin());
        }
    }

    void OnGUI() override
    {
        ImGui::Begin("Fourier Parameters");

        ImGui::SliderInt("Max Harmonics", &maxHarmonics, 1, 50);
        ImGui::SliderInt("Radius Scale", &radiusScale, 10.0f, 200.0f);
        ImGui::SliderFloat("Speed", &speed, 0.01f, 1.0f);

        ImGui::End();
    }
};