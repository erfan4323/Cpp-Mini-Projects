#pragma once

#include <vector>
#include "PerlinNoise.h"

#include "Game.h"
#include <raymath.h>

constexpr auto TWO_PI = PI * 2;

class PerlinLoops : public Game
{
private:
	int width;
	int height;
	Vector2 center;

	std::vector<Vector2> points;
	std::vector<Vector2> prevPoints;

	int noiseMax = 120;
	PerlinNoise noise;
	float zoff = 0;

	float interpolation = 0.0f;
	const float interpolationSpeed = 0.3f;

public:
	PerlinLoops(std::string windowName, int width, int height) :
		Game(windowName, width, height),
		width(width),
		height(height),
		center(Vector2{(float) width / 2, (float) height / 2})
	{}
	~PerlinLoops() {}

private:
	void LoopGen()
	{
		if (points.empty())
			prevPoints.resize(points.size());
		else
			prevPoints = points;

		points.clear();
		for (float a = 0; a < TWO_PI; a += 0.05f)
		{
			float xoff = Remap(cos(a), -1, 1, 0, noiseMax);
			float yoff = Remap(sin(a), -1, 1, 0, noiseMax);
			float r = Remap(noise.noise(xoff, yoff, zoff), 0, 1, 100, 200);
			auto x = center.x + r * sin(a);
			auto y = center.y + r * cos(a);
			points.push_back(Vector2{x, y});
		}
		points.push_back(points.front());
		zoff += 0.2;
		interpolation = 0.0f;
	}

	void interpolatePoints(std::vector<Vector2>& interpolatedPoints)
	{
		interpolatedPoints.reserve(points.size());
		for (size_t i = 0; i < points.size(); ++i)
		{
			float x = prevPoints[i].x + interpolation * (points[i].x - prevPoints[i].x);
			float y = prevPoints[i].y + interpolation * (points[i].y - prevPoints[i].y);
			interpolatedPoints.push_back({x, y});
		}
	}
	void updateInterpolation()
	{
		if (interpolation < 1.0f)
		{
			interpolation += interpolationSpeed;

			if (interpolation > 1.0f)
				interpolation = 1.0f;
		}
	}

private:
	void OnCreate() override
	{
		LoopGen();
		prevPoints = points;
	}

	void Update(float dt) override
	{
		updateInterpolation();
	}

	void Render() override
	{
		std::vector<Vector2> interpolatedPoints;
		interpolatePoints(interpolatedPoints);

		DrawSplineLinear(interpolatedPoints.data(), interpolatedPoints.size(), 2, RAYWHITE);

		if (interpolation >= 1.0f)
			LoopGen();
	}
};