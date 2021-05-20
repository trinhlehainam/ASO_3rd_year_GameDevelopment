#pragma once

namespace MathHelper
{
	// Floating point numbers are rarely exact the same.
	// -> Two floating point numbers are equal when they are in range of below bias
	extern const float kBiasF;
	extern const double kBiasD;
	//

	extern const float kMsToSecond;
}

namespace MathHelper
{
	float randf(float num);
	float randf(float min, float max);
	template<typename T> bool isEqual(T a, T b);
};

