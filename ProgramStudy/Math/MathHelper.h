#pragma once

namespace MathHelper
{
	extern const float kBiasF;
	extern const double kBiasD;
}

namespace MathHelper
{
	float randf(float num);
	float randf(float min, float max);
	template<typename T> bool isEqual(T a, T b);
};

