#pragma once
#include "VectorExt.h"

class Particle
{
private:
	int width;
	int height;
	int scale;
	int maxSpeed;

	Vector2 pos;
	Vector2 vel;
	Vector2 acc;
	Vector2 prevPos;

public:
	Particle() = default;

	Particle(int width, int height, int scale) :
		width(width), height(height),
		pos({(float) GetRandomValue(0, width), (float) GetRandomValue(0, height)}),
		vel(GenerateRandomDirection()),
		acc({0, 0}),
		scale(scale),
		maxSpeed(2),
		prevPos(pos)
	{}

	~Particle()
	{}

	void Update()
	{
		vel = Vector2Add(vel, acc);
		vel = Vector2Limit(vel, maxSpeed);
		pos = Vector2Add(pos, vel);
		acc = {0, 0};
	}

	void ApplyForce(Vector2 force)
	{
		acc = Vector2Add(acc, force);
	}

	void UpdatePrev()
	{
		prevPos = pos;
	}

	void Show()
	{
		DrawLine(pos.x, pos.y, prevPos.x, prevPos.y, {245, 245, 245, 5});
		UpdatePrev();
	}

	void Edges()
	{
		if (pos.x > width)
		{
			pos.x = 0;
			UpdatePrev();
		}
		if (pos.x < 0)
		{
			pos.x = width;
			UpdatePrev();
		}
		if (pos.y > height)
		{
			pos.y = 0;
			UpdatePrev();
		}
		if (pos.y < 0)
		{
			pos.y = height;
			UpdatePrev();
		}
	}

	void Follow(std::vector<Vector2> vectors)
	{
		auto cols = floor((float) width / scale);
		auto x = floor(pos.x / scale);
		auto y = floor(pos.y / scale);
		auto index = static_cast<size_t>(x + y * cols);
		if (index < vectors.size())
		{
			auto& force = vectors[index];
			ApplyForce(force);
		}
	}

	static void InitializeRandom()
	{
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
	}
};

class ParticleManager
{
private:
	std::vector<Particle> particles;

public:
	void Initialize(int numParticles, int width, int height, int scale)
	{
		particles.resize(numParticles);
		for (auto& particle : particles)
		{
			particle = Particle(width, height, scale);
		}
	}

	void Update(const std::vector<Vector2>& flowField)
	{
		for (auto& particle : particles)
		{
			particle.Follow(flowField);
			particle.Update();
			particle.Edges();
		}
	}

	void RenderParticles()
	{
		for (auto& particle : particles)
			particle.Show();
	}
};