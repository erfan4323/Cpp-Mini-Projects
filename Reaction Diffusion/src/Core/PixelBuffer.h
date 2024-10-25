#include <raylib.h>
#include <memory>

struct TextureBuffer
{
    Texture2D texture;
    std::unique_ptr<Color[]> pixels;
    Color backgroundColor;
    int width;
    int height;

    TextureBuffer(int width, int height, Color bgColor)
        : width(width), height(height), backgroundColor(bgColor),
        pixels(std::make_unique<Color[]>(width* height))
    {
        InitializePixels(bgColor);
        CreateTexture();
    }

    ~TextureBuffer()
    {
        UnloadTexture(texture);
    }

    void InitializePixels(Color bgColor)
    {
        std::fill(pixels.get(), pixels.get() + (width * height), bgColor);
    }

    void CreateTexture()
    {
        texture = LoadTextureFromImage(GenImageColor(width, height, backgroundColor));
        UpdateTexture(texture, pixels.get());
    }

    void UpdateAndDrawTexture(Color bgColor)
    {
        UpdateTexture(texture, pixels.get());

        DrawTexture(texture, 0, 0, bgColor);
    }

    void UpdateAndDrawTexture()
    {
        UpdateAndDrawTexture(backgroundColor);
    }
};