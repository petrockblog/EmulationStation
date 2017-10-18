#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <assert.h>
#include "math/CommaInitializer.h"

namespace Eigen {

template<typename S>
class Vector2
{
public:

	typedef Vector2<S> V;

	Vector2() { }
	Vector2(const S t) : mX(t), mY(t) { };
	Vector2(const S x, const S y) : mX(x), mY(y) { };

	const bool operator==(const V& other) const { return ((mX == other.x()) && (mY == other.y())); }
	const bool operator!=(const V& other) const { return ((mX != other.x()) || (mY != other.y())); }

	const V operator+(const V& other) const { return { mX + other.x(), mY + other.y() }; }
	const V operator-(const V& other) const { return { mX - other.x(), mY - other.y() }; }
	const V operator*(const V& other) const { return { mX * other.x(), mY * other.y() }; }
	const V operator/(const V& other) const { return { mX / other.x(), mY / other.y() }; }

	const V operator+(const S& other) const { return { mX + other, mY + other }; }
	const V operator-(const S& other) const { return { mX - other, mY - other }; }
	const V operator*(const S& other) const { return { mX * other, mY * other }; }
	const V operator/(const S& other) const { return { mX / other, mY / other }; }

	const V operator-() const { return { -mX , -mY }; }

	V& operator+=(const V& other) { *this = *this + other; return *this; }
	V& operator-=(const V& other) { *this = *this - other; return *this; }
	V& operator*=(const V& other) { *this = *this * other; return *this; }
	V& operator/=(const V& other) { *this = *this / other; return *this; }

	V& operator+=(const S& other) { *this = *this + other; return *this; }
	V& operator-=(const S& other) { *this = *this - other; return *this; }
	V& operator*=(const S& other) { *this = *this * other; return *this; }
	V& operator/=(const S& other) { *this = *this / other; return *this; }

	S& operator[](const size_t index) { assert(index < Size() && "index out of range"); return mV[index]; }
	const S& operator[](const size_t index) const { assert(index < Size() && "index out of range"); return mV[index]; }

	CommaInitializer<V, S> V::operator<<(const S& s) { return CommaInitializer<V, S>(*this, s); }

	S& x() { return mX; }
	S& y() { return mY; }
	const S& x() const { return mX; }
	const S& y() const { return mY; }

	S* data() { return (S*)this; }
	const S* data() const { return (S*)this; }

	void set(const S x, const S y) { mX = x; mY = y; }
	bool isZero() const { return *this == Zero(); }
	const V cwiseProduct(const V& other) const { return *this * other; }
	template<typename T> Vector2<T> cast() const { return Vector2<T>((T)mX, (T)mY); }

	static const size_t Size() { return 2; }
	static const V Zero() { return { 0, 0 }; }
	static const V UnitX() { return { 1, 0 }; }
	static const V UnitY() { return { 0, 1 }; }

private:

	union
	{
		struct { S mX; S mY; };
		S mV[2];
	};
};

typedef Vector2<bool>   Vector2b;
typedef Vector2<int>    Vector2i;
typedef Vector2<float>  Vector2f;
typedef Vector2<double> Vector2d;

}

#endif
