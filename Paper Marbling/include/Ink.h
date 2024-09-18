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

	void Tine(Vector2 m, Vector2 b, float z, float c)
	{
		auto u = 1 / pow(2, 1 / c);
		for (auto& vert : vertices)
			TineMath(vert, b, m, z, u);

		TineMath(center, b, m, z, u);
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

	void TineMath(Vector2& vert, const Vector2& b, const Vector2& m, float z, double u)
	{
		auto pminb = Vector2Subtract(vert, b);
		auto n = Vector2Rotate(m, HALF_PI);
		auto d = abs(Vector2DotProduct(pminb, n));
		auto mag = z * pow(u, d);
		auto p = Vector2Add(vert, Vector2Scale(m, mag));
		vert = p;
	}
};
