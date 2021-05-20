#include "MathHelper.h"

#include <cmath>

namespace MathHelper
{
	const float kBiasF = 0.001f;
	const double kBiasD = 0.0001;

	const float kMsToSecond = 1 / 1000.0f;
}

namespace MathHelper
{
	float randf(float num)
	{
		return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / num));
	}

	float randf(float min, float max)
	{
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max - min));
	}

	template<typename T>
	bool isEqual(T a, T b)
	{
		return a == b;
	}

#pragma region Specialization
	template <>
	bool isEqual(float a, float b)
	{
		return std::fabsf(a - b) <= kBiasF;
	}

	template <>
	bool isEqual(double a, double b)
	{
		return std::abs(a - b) <= kBiasD;
	}
#pragma endregion


#pragma region Instantiate
	template bool isEqual(int, int);
	template bool isEqual(unsigned int, unsigned int);
	template bool isEqual(short, short);
#pragma endregion

};
