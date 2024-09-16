#pragma once
#include "raylib.h"
#include "rayutils.h"
#include <iostream>

class Drop
{
private:
	int circleRes;
	Color color;
	std::vector<Vector2> vertices;

public:
	int radius;
	Vector2 center;

	Drop(int x, int y, int r) :
		radius(r),
		circleRes(100),
		center({(float) x, (float) y})
	{
		SetupVertices(r);
		color = GetRandomColor();
	}

	~Drop()
	{}

	void Marble(Drop other)
	{
		for (auto& vert : vertices)
			MarbleMath(other, vert);

		MarbleMath(other, center);
	}

	void Draw()
	{
		DrawPolyPoint(center, vertices.data(), vertices.size(), color);
	}

private:
	void SetupVertices(int r)
	{
		for (int i = 0; i < circleRes; i++)
		{
			auto angle = Remap(i, 0, circleRes, 0, TWO_PI);
			Vector2 ver = {cos(angle), sin(angle)};
			ver = Vector2Scale(ver, r);
			ver = Vector2Add(ver, center);
			vertices.push_back(ver);
		}
	}

	void MarbleMath(Drop& other, Vector2& vert)
	{
		// marble math :
		// C + (P - C) * sqrt(1 + (r^2/|P - C|^2))
		auto c = other.center;
		auto& p = vert;
		auto r = other.radius;
		auto pminc = Vector2Subtract(p, c);
		auto m = Vector2LengthSqr(pminc);
		auto root = sqrt(1 + (r * r) / m);
		auto result = Vector2Scale(pminc, root);
		result = Vector2Add(result, c);
		vert = result;
	}
};
