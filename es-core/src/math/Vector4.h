#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include <assert.h>
#include "math/Vector3.h"
#include "math/CommaInitializer.h"

namespace Eigen {

template<typename S>
class Vector4
{
public:

	typedef Vector4<S> V;

	Vector4() { }
	Vector4(const S t) : mX(t), mY(t), mZ(t), mW(t) { };
	Vector4(const S x, const S y, const S z, const S w) : mX(x), mY(y), mZ(z), mW(w) { };

	const bool operator==(const V& other) const { return ((mX == other.x()) && (mY == other.y()) && (mZ == other.z()) && (mW == other.w())); }
	const bool operator!=(const V& other) const { return ((mX != other.x()) || (mY != other.y()) || (mZ != other.z()) || (mW != other.w())); }

	const V operator+(const V& other) const { return { mX + other.x(), mY + other.y(), mZ + other.z(), mW + other.w() }; }
	const V operator-(const V& other) const { return { mX - other.x(), mY - other.y(), mZ - other.z(), mW - other.w() }; }
	const V operator*(const V& other) const { return { mX * other.x(), mY * other.y(), mZ * other.z(), mW * other.w() }; }
	const V operator/(const V& other) const { return { mX / other.x(), mY / other.y(), mZ / other.z(), mW / other.w() }; }

	const V operator+(const S& other) const { return { mX + other, mY + other, mZ + other, mW + other }; }
	const V operator-(const S& other) const { return { mX - other, mY - other, mZ - other, mW - other }; }
	const V operator*(const S& other) const { return { mX * other, mY * other, mZ * other, mW * other }; }
	const V operator/(const S& other) const { return { mX / other, mY / other, mZ / other, mW / other }; }

	const V operator-() const { return { -mX , -mY, -mZ, -mW }; }

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

	CommaInitializer<V, S> V::operator<<(const S& t);

	S& x() { return mX; }
	S& y() { return mY; }
	S& z() { return mZ; }
	S& w() { return mW; }
	const S& x() const { return mX; }
	const S& y() const { return mY; }
	const S& z() const { return mZ; }
	const S& w() const { return mW; }

	Vector3<S>& v3() { return *(Vector3<S>*)this; }
	const Vector3<S>& v3() const { return *(Vector3<S>*)this; }

	S* data() { return (S*)this; }
	const S* data() const { return (S*)this; }

	void set(const S x, const S y, const S z, const S w) { mX = x; mY = y; mZ = z; mW = w; }
	bool isZero() const { return *this == Zero(); }
	const V cwiseProduct(const V& other) const { return *this * other; }
	template<typename T> Vector4<T> cast() const { return Vector4<T>((T)mX, (T)mY); }

	static const size_t Size() { return 4; }
	static const V Zero() { return { 0, 0, 0, 0 }; }
	static const V UnitX() { return { 1, 0, 0, 0 }; }
	static const V UnitY() { return { 0, 1, 0, 0 }; }
	static const V UnitZ() { return { 0, 0, 1, 0 }; }
	static const V UnitW() { return { 0, 0, 0, 1 }; }

private:

	union
	{
		struct { S mX; S mY; S mZ; S mW; };
		S mV[4];
	};
};

typedef Vector4<bool>   Vector4b;
typedef Vector4<int>    Vector4i;
typedef Vector4<float>  Vector4f;
typedef Vector4<double> Vector4d;

}

#endif
