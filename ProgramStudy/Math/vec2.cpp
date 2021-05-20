#include "vec2.h"

#include "MathHelper.h"

#include <cmath>
#include <cassert>

template<typename T>
vec2<T>::vec2():x(0),y(0)
{
}


template<typename T>
vec2<T>::vec2(T x, T y):x(x),y(y)
{
}

template<typename T>
vec2<T>::vec2(const vec2& other)
{
	x = other.x;
	y = other.y;
}

template<typename T>
vec2<T>::~vec2() = default;

template<typename T>
vec2<T>& vec2<T>::operator=(const vec2& other)
{
	x = other.x;
	y = other.y;

	return *this;
}

template<typename T>
vec2<T>& vec2<T>::operator+=(const vec2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

template<typename T>
vec2<T>& vec2<T>::operator-=(const vec2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

template<typename T>
vec2<T>& vec2<T>::operator*=(int scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

template<typename T>
vec2<T>& vec2<T>::operator/=(int scalar)
{
	assert(scalar != 0);
	x /= scalar;
	y /= scalar;
	return *this;
}

template<typename T>
T& vec2<T>::operator[](unsigned int i)
{
	assert(i >= 0 && i < 2);
	auto ret = reinterpret_cast<T*>(this);
	return *(ret + i);
}

template<typename T>
vec2<T> vec2<T>::operator-() const
{
	return vec2(-x, -y);
}

template<typename T>
vec2<T> vec2<T>::operator+() const
{
	return *this;
}

template<typename T>
vec2<T> operator+(const vec2<T>& a, const vec2<T>& b)
{
	return vec2<T>(a.x + b.x, a.y + b.y);
}

template<typename T>
vec2<T> operator-(const vec2<T>& a, const vec2<T>& b)
{
	return vec2<T>(a.x - b.x, a.y - b.y);
}

template<typename T>
vec2<T> operator*(const vec2<T>& a, T scalar)
{
	return vec2<T>(a.x * scalar, a.y * scalar);
}

template<typename T>
vec2<T> operator*(T scalar, const vec2<T>& a)
{
	return vec2<T>(a.x * scalar, a.y * scalar);
}

template<typename T>
T operator*(const vec2<T> a, const vec2<T>& b)
{
	return a.x * b.x + a.y * b.y;
}

template<typename T>
vec2<T> operator/(const vec2<T>& a, T scalar)
{
	assert(scalar != 0);
	return vec2<T>(a.x / scalar, a.y / scalar);
}

template<typename T>
vec2<T> operator%(const vec2<T>& a, int scalar)
{
	assert(scalar != 0);
	return vec2<T>(static_cast<T>(static_cast<int>(a.x) % scalar), static_cast<T>(static_cast<int>(a.y) % scalar));
}

template<typename T>
bool operator==(const vec2<T>& a, T value)
{
	return a.x == value && a.y == value;
}

template<typename T>
bool operator==(const vec2<T>& a, const vec2<T>& b)
{
	return a.x == b.x && a.y == b.y;
}

template<typename T>
bool operator!=(const vec2<T>& a, const vec2<T>& b)
{
	return a.x != b.x || a.y != b.y;
}

template<typename T>
bool operator>=(const vec2<T>& a, const vec2<T>& b)
{
	return a.x >= b.x && a.y >= b.y;
}

template<typename T>
bool operator>(const vec2<T>& a, const vec2<T>& b)
{
	return a.x > b.x && a.y > b.y;
}

template<typename T>
bool operator<=(const vec2<T>& a, const vec2<T>& b)
{
	return a.x <= b.x && a.y <= b.y;
}

template<typename T>
bool operator<(const vec2<T>& a, const vec2<T>& b)
{
	return a.x < b.x && a.y < b.y;
}

template<typename T>
T dot(const vec2<T>& a, const vec2<T>& b)
{
	return a.x * b.x + a.y * b.y;
}

template<typename T>
vec2<T> reflectionVec(const vec2<T>& i, const vec2<T>& n)
{
	return i - static_cast<T>(2) * (n * i) * n;
}

template<typename T>
vec2<T> projectVec(const vec2<T>& v, const vec2<T>& onto)
{
	return onto*(dot(v,onto)/dot(onto,onto));
}

template<typename T>
vec2<T> orthogonalVec(const vec2<T>& v)
{
	return vec2<T>(-v.y, v.x);
}

template<typename T>
bool isParallelVec(const vec2<T>& a, const vec2<T>& b)
{
	return dot(orthogonalVec(a), b) == 0;
}

#pragma region Specialization
template <>
vec2u vec2u::operator - () const
{
	assert(0);			// Unsgined number can't be negative
	return vec2u(x, y);
}

template <>
bool operator == (const vec2f& a, float value)
{
	return MathHelper::isEqual(a.x,value) && MathHelper::isEqual(a.y, value);
}

template <> 
bool operator == (const vec2f& a, const vec2f& b)
{
	auto vec_dif = a - b;
	return vec_dif == MathHelper::kBiasF;
}

template <>
bool isParallelVec(const vec2f& a, const vec2f& b)
{
	return MathHelper::isEqual(dot(orthogonalVec(a), b), 0.0f);
}
#pragma endregion


#pragma region Instantiate

template vec2<int>;
template vec2<unsigned int>;
template vec2<float>;
template vec2<double>;

#pragma region int
template vec2i operator + (const vec2i& a, const vec2i& b);
template vec2i operator - (const vec2i& a, const vec2i& b);
template vec2i operator * (const vec2i& a, int scalar);
template vec2i operator * (int scalar, const vec2i& a);
template vec2i operator / (const vec2i& a, int scalar);
template vec2i operator % (const vec2i& a, int scalar);

template bool operator == (const vec2i& a, const vec2i& b);
template bool operator != (const vec2i& a, const vec2i& b);
template bool operator >= (const vec2i& a, const vec2i& b);
template bool operator >  (const vec2i& a, const vec2i& b);
template bool operator <= (const vec2i& a, const vec2i& b);
template bool operator <  (const vec2i& a, const vec2i& b);
#pragma endregion

#pragma region unsigned int
template vec2u operator + (const vec2u& a, const vec2u& b);
template vec2u operator - (const vec2u& a, const vec2u& b);
template vec2u operator * (const vec2u& a, unsigned int scalar);
template vec2u operator * (unsigned int scalar, const vec2u& a);
template vec2u operator / (const vec2u& a, unsigned int scalar);
template vec2u operator % (const vec2u& a, int scalar);

template bool operator == (const vec2u& a, const vec2u& b);
template bool operator != (const vec2u& a, const vec2u& b);
template bool operator >= (const vec2u& a, const vec2u& b);
template bool operator >  (const vec2u& a, const vec2u& b);
template bool operator <= (const vec2u& a, const vec2u& b);
template bool operator <  (const vec2u& a, const vec2u& b);
#pragma endregion

#pragma region float
template vec2f operator + (const vec2f& a, const vec2f& b);
template vec2f operator - (const vec2f& a, const vec2f& b);
template vec2f operator * (const vec2f& a, float scalar);
template vec2f operator * (float scalar, const vec2f& a);
template vec2f operator / (const vec2f& a, float scalar);
template vec2f operator % (const vec2f& a, int scalar);

template bool operator == (const vec2f& a, const vec2f& b);
template bool operator != (const vec2f& a, const vec2f& b);
template bool operator >= (const vec2f& a, const vec2f& b);
template bool operator >  (const vec2f& a, const vec2f& b);
template bool operator <= (const vec2f& a, const vec2f& b);
template bool operator <  (const vec2f& a, const vec2f& b);

template float dot(const vec2f& a, const vec2f& b);
template vec2f reflectionVec(const vec2f& i, const vec2f& n);
template vec2f projectVec(const vec2f& v, const vec2f& n);
template vec2f orthogonalVec(const vec2f& v);
#pragma endregion

#pragma region double
template vec2d operator + (const vec2d& a, const vec2d& b);
template vec2d operator - (const vec2d& a, const vec2d& b);
template vec2d operator * (const vec2d& a, double scalar);
template vec2d operator * (double scalar, const vec2d& a);
template vec2d operator / (const vec2d& a, double scalar);
template vec2d operator % (const vec2d& a, int scalar);

template bool operator == (const vec2d& a, const vec2d& b);
template bool operator != (const vec2d& a, const vec2d& b);
template bool operator >= (const vec2d& a, const vec2d& b);
template bool operator >  (const vec2d& a, const vec2d& b);
template bool operator <= (const vec2d& a, const vec2d& b);
template bool operator <  (const vec2d& a, const vec2d& b);
template double dot(const vec2d& a, const vec2d& b);
template vec2d reflectionVec(const vec2d& i, const vec2d& n);
template vec2d projectVec(const vec2d& vi, const vec2d& onto);
template vec2d orthogonalVec(const vec2d& v);
#pragma endregion

#pragma endregion

