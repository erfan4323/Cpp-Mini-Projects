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
	Drop(int x, int y, int r, Color col) :
		circleRes(500),
		radius(r),
		center({(float) x, (float) y}),
		color(col)
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

	void Tine(Vector2 mouseMovement, Vector2 basePosition, float intensity, float decayExponent)
	{
		auto decayFactor = 1 / pow(2, 1 / decayExponent);
		for (auto& vertex : vertices)
			TineMath(vertex, basePosition, mouseMovement, intensity, decayFactor);

		TineMath(center, basePosition, mouseMovement, intensity, decayFactor);
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

	void TineMath(
		Vector2& vertex, 
		const Vector2& basePosition, 
		const Vector2& mouseMovement, 
		float intensity, 
		double decayFactor
	)
	{
		auto offset = Vector2Subtract(vertex, basePosition);
		auto perpendicularDirection = Vector2Rotate(mouseMovement, HALF_PI);
		auto distance = abs(Vector2DotProduct(offset, perpendicularDirection));
		auto magnitude = intensity * pow(decayFactor, distance);
		auto newPosition = Vector2Add(vertex, Vector2Scale(mouseMovement, magnitude));
		vertex = newPosition;
	}
};
