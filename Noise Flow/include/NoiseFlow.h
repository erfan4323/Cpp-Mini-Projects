#pragma once
#include "Game.h"
#include "PerlinNoise.h"
#include "Particle.h"
#include "rayutils.h"
#include <vector>

class FlowFieldGenerator
{
public:
	virtual ~FlowFieldGenerator() = default;
	virtual Vector2 GenerateVector(float x, float y, float z) = 0;
};

class PerlinFlowFieldGenerator : public FlowFieldGenerator
{
private:
	PerlinNoise noise;
	float scale;
public:
	PerlinFlowFieldGenerator(float scale) : scale(scale) {}

	Vector2 GenerateVector(float x, float y, float z) override
	{
		float angle = noise.noise(x, y, z) * TWO_PI * 5;
		Vector2 vec = FromAngle(angle);
		vec = Vector2SetMag(vec, 0.30);
		return vec;
	}
};

class NoiseFlow : public Game
{
private:
	int width;
	int height;
	int scale = 10;
	float rows;
	float cols;
	float zoff = 0.0f;

	std::unique_ptr<FlowFieldGenerator> flowFieldGenerator;
	std::vector<Vector2> flowField;
	ParticleManager particleManager;
	RenderTexture2D renderTexture;
public:
	NoiseFlow(std::string windowName, int width, int height) :
		Game(windowName, width, height),
		width(width),
		height(height)
	{
		flowFieldGenerator = std::make_unique<PerlinFlowFieldGenerator>(0.1f);
	}

	~NoiseFlow()
	{
		UnloadRenderTexture(renderTexture);
	}

private:
	void OnCreate() override
	{
		Particle::InitializeRandom();
		cols = floor((float) width / scale);
		rows = floor((float) height / scale);

		particleManager.Initialize(1000, width, height, scale);
		flowField.resize(static_cast<size_t>(cols * rows));

		renderTexture = LoadRenderTexture(width, height);

		renderTexture = LoadRenderTexture(width, height);

		BeginTextureMode(renderTexture);
		ClearBackground(Color{53, 53, 53, 255});
		EndTextureMode();
	}

	void Update(float dt) override
	{
		float yoff = 0.0f;

		for (int row = 0; row < rows; ++row)
		{
			float xoff = 0.0f;
			for (int col = 0; col < cols; ++col)
			{
				auto index = col + row * cols;
				flowField[index] = flowFieldGenerator->GenerateVector(xoff, yoff, zoff);
				xoff += 0.1f;
			}
			yoff += 0.1f;
		}

		zoff += 0.003f;

		particleManager.Update(flowField);
	}

	void Render() override
	{
		BeginTextureMode(renderTexture);
		particleManager.RenderParticles();
		EndTextureMode();

		DrawTexture(renderTexture.texture, 0, 0, WHITE);
	}
};
