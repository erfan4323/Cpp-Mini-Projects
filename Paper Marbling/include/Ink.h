#pragma once
#include "Common/rayutils.h"
#include <iostream>

class Drop
{
private:
	int circleRes;
	Color color;
	std::vector<Vector2> vertices;
	int radius;
	Vector2 center;

public:

	Drop(int x, int y, int r) :
		circleRes(500),
		radius(r),
		center({(float) x, (float) y}),
		color(GetRandomColor())
	{
		SetupVertices(r);
	}

	~Drop()
	{}

	void Marble(Drop other)
	{
		for (auto& vert : vertices)
			MarbleMath(other, vert);

		MarbleMath(other, center);
	}

	void Tine(float xPos, float z, float c)
	{
		auto u = 1 / pow(2, 1 / c);

		for (auto& vert : vertices)
			TineMath(vert, z, u, xPos);

		TineMath(center, z, u, xPos);
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
		// C is the center of the drop we are putting and r is the radius of that drop
		// P is every point on other drops that should move in accord of the C and it's radius r
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

	void TineMath(Vector2& vert, float z, double u, float xPos)
	{
		vert.x = vert.x;
		vert.y = vert.y + z * pow(u, abs(vert.x - xPos));
	}
};
