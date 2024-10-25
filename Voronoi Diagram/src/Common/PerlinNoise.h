#pragma once

#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

class PerlinNoise
{
public:
	PerlinNoise()
		: perlin_octaves(4), perlin_amp_falloff(0.8f), perlin_random(std::random_device{}())
	{
		initialize();
	}

	float noise(float x)
	{
		return noise(x, 0.0f, 0.0f);
	}

	/**
	 * @brief Generates Perlin noise for a 2D input.
	 * @return The Perlin noise value at the given (x, y).
	 */
	float noise(float x, float y)
	{
		return noise(x, y, 0.0f);
	}

	/**
	 * @brief Generates Perlin noise for a 3D input.
	 * @return The Perlin noise value at the given (x, y, z).
	 */
	float noise(float x, float y, float z)
	{
		if (perlin.empty())
		{
			initialize();
		}

		if (x < 0) x = -x;
		if (y < 0) y = -y;
		if (z < 0) z = -z;

		int xi = static_cast<int>(x) & PERLIN_SIZE;
		int yi = static_cast<int>(y) & PERLIN_SIZE;
		int zi = static_cast<int>(z) & PERLIN_SIZE;
		float xf = x - static_cast<int>(x);
		float yf = y - static_cast<int>(y);
		float zf = z - static_cast<int>(z);
		float rxf;
		float ryf;

		float r = 0.0f;
		float ampl = 0.5f;

		for (int i = 0; i < perlin_octaves; i++)
		{
			int of = xi + (yi << PERLIN_YWRAPB) + (zi << PERLIN_ZWRAPB);

			rxf = noise_fsc(xf);
			ryf = noise_fsc(yf);

			float n1;
			float n2;
			float n3;
			n1 = perlin[of & PERLIN_SIZE];
			n1 += rxf * (perlin[(of + 1) & PERLIN_SIZE] - n1);
			n2 = perlin[(of + PERLIN_YWRAP) & PERLIN_SIZE];
			n2 += rxf * (perlin[(of + PERLIN_YWRAP + 1) & PERLIN_SIZE] - n2);
			n1 += ryf * (n2 - n1);

			of += PERLIN_ZWRAP;
			n2 = perlin[of & PERLIN_SIZE];
			n2 += rxf * (perlin[(of + 1) & PERLIN_SIZE] - n2);
			n3 = perlin[(of + PERLIN_YWRAP) & PERLIN_SIZE];
			n3 += rxf * (perlin[(of + PERLIN_YWRAP + 1) & PERLIN_SIZE] - n3);
			n2 += ryf * (n3 - n2);

			n1 += noise_fsc(zf) * (n2 - n1);

			r += n1 * ampl;
			ampl *= perlin_amp_falloff;
			xi <<= 1; xf *= 2;
			yi <<= 1; yf *= 2;
			zi <<= 1; zf *= 2;

			if (xf >= 1.0f) { xi++; xf--; }
			if (yf >= 1.0f) { yi++; yf--; }
			if (zf >= 1.0f) { zi++; zf--; }
		}
		return r;
	}

	/**
	 * @brief Sets the level of detail (LOD) for Perlin noise.
	 * @param lod The number of octaves to use for the noise calculation.
	 */
	void noiseDetail(int lod)
	{
		if (lod > 0) perlin_octaves = lod;
	}

	/**
	 * @brief Sets the level of detail (LOD) and falloff for Perlin noise.
	 * @param lod The number of octaves to use for the noise calculation.
	 * @param falloff The amplitude falloff factor for each octave.
	 */
	void noiseDetail(int lod, float falloff)
	{
		if (lod > 0) perlin_octaves = lod;
		if (falloff > 0) perlin_amp_falloff = falloff;
	}

	/**
	 * @brief Seeds the random number generator and reinitializes the noise.
	 * @param seed The seed value for the random number generator.
	 */
	void noiseSeed(unsigned long seed)
	{
		perlin_random.seed(seed);
		initialize();
	}

private:
	static const int PERLIN_YWRAPB = 4;
	static const int PERLIN_YWRAP = 1 << PERLIN_YWRAPB;
	static const int PERLIN_ZWRAPB = 8;
	static const int PERLIN_ZWRAP = 1 << PERLIN_ZWRAPB;
	static const int PERLIN_SIZE = 4095;

	int perlin_octaves;
	float perlin_amp_falloff;

	std::vector<float> perlin;
	std::vector<float> perlin_cosTable;
	int perlin_TWOPI;
	int perlin_PI;

	std::default_random_engine perlin_random;
	std::uniform_real_distribution<float> distribution;

	void initialize()
	{
		perlin.resize(PERLIN_SIZE + 1);
		for (auto& val : perlin)
		{
			val = distribution(perlin_random);
		}
		perlin_cosTable.resize(360);
		perlin_TWOPI = 360; // This depends on the size of the cosine table
		perlin_PI = perlin_TWOPI / 2;
	}

	float noise_fsc(float i)
	{
		return 0.5f * (1.0f - perlin_cosTable[static_cast<int>(i * perlin_PI) % perlin_TWOPI]);
	}
};
