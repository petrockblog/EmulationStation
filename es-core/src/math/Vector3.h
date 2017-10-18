#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <assert.h>
#include "math/CommaInitializer.h"

namespace Eigen {

template<typename S>
class Vector3
{
public:

	typedef Vector3<S> V;

	Vector3() { }
	Vector3(const S t) : mX(t), mY(t), mZ(t) { };
	Vector3(const S x, const S y, const S z) : mX(x), mY(y), mZ(z) { };

	const bool operator==(const V& other) const { return ((mX == other.x()) && (mY == other.y()) && (mZ == other.z())); }
	const bool operator!=(const V& other) const { return ((mX != other.x()) || (mY != other.y()) || (mZ != other.z())); }

	const V operator+(const V& other) const { return { mX + other.x(), mY + other.y(), mZ + other.z() }; }
	const V operator-(const V& other) const { return { mX - other.x(), mY - other.y(), mZ - other.z() }; }
	const V operator*(const V& other) const { return { mX * other.x(), mY * other.y(), mZ * other.z() }; }
	const V operator/(const V& other) const { return { mX / other.x(), mY / other.y(), mZ / other.z() }; }

	const V operator+(const S& other) const { return { mX + other, mY + other, mZ + other }; }
	const V operator-(const S& other) const { return { mX - other, mY - other, mZ - other }; }
	const V operator*(const S& other) const { return { mX * other, mY * other, mZ * other }; }
	const V operator/(const S& other) const { return { mX / other, mY / other, mZ / other }; }

	const V operator-() const { return { -mX , -mY, -mZ }; }

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

	CommaInitializer<V, S> operator<<(const S& s) { return CommaInitializer<V, S>(*this, s); }

	S& x() { return mX; }
	S& y() { return mY; }
	S& z() { return mZ; }
	const S& x() const { return mX; }
	const S& y() const { return mY; }
	const S& z() const { return mZ; }

	S* data() { return (S*)this; }
	const S* data() const { return (S*)this; }

	void set(const S x, const S y, const S z) { mX = x; mY = y; mZ = z; }
	bool isZero() const { return *this == Zero(); }
	const V cwiseProduct(const V& other) const { return *this * other; }
	template<typename T> Vector3<T> cast() const { return Vector3<T>((T)mX, (T)mY); }

	static const size_t Size() { return 3; }
	static const V Zero() { return { 0, 0, 0 }; }
	static const V UnitX() { return { 1, 0, 0 }; }
	static const V UnitY() { return { 0, 1, 0 }; }
	static const V UnitZ() { return { 0, 0, 1 }; }

private:

	union
	{
		struct { S mX; S mY; S mZ; };
		S mV[3];
	};
};

typedef Vector3<bool>   Vector3b;
typedef Vector3<int>    Vector3i;
typedef Vector3<float>  Vector3f;
typedef Vector3<double> Vector3d;

}

#endif
