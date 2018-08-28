#pragma once
#include <random>
class Random
{
private:
	std::mt19937 mt;

public:
	Random()
	{
		std::random_device device;
		mt.seed(device());
	}
	int GetRand(int min, int max)
	{
		std::uniform_int_distribution<int> range(min, max);
		return range(mt);
	}
	float GetRand(float min, float max)
	{
		std::uniform_real_distribution<float> range(min, max);
		return range(mt);
	}
	double GetRand(double min, double max)
	{
		std::uniform_real_distribution<double> range(min, max);
		return range(mt);
	}
};