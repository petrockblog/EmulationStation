#ifndef _MATRIX4X4_H_
#define _MATRIX4X4_H_

#include <assert.h>
#include <math/Vector4.h>
#include <math/Vector3.h>

template<typename S>
class Matrix4x4
{
public:

	typedef Matrix4x4<S> M;

	Matrix4x4() { }
	Matrix4x4(const Vector4<S> x, const Vector4<S> y, const Vector4<S> z, const Vector4<S> w) : mX(x), mY(y), mZ(z), mW(w) { };

	const bool operator==(const M& other) const { return ((mX == other.x()) && (mY == other.y()) && (mZ == other.z()) && (mW == other.w())); }
	const bool operator!=(const M& other) const { return ((mX != other.x()) || (mY != other.y()) || (mZ != other.z()) || (mW != other.w())); }

	const M operator+(const M& other) const { return { mX + other.x(), mY + other.y(), mZ + other.z(), mW + other.w() }; }
	const M operator-(const M& other) const { return { mX - other.x(), mY - other.y(), mZ - other.z(), mW - other.w() }; }

	const M operator*(const M& other) const
	{
		return
		{
			{ other.x() * mX.x() + other.y() * mX.y() + other.z() * mX.z() + other.w() * mX.w() },
			{ other.x() * mY.x() + other.y() * mY.y() + other.z() * mY.z() + other.w() * mY.w() },
			{ other.x() * mZ.x() + other.y() * mZ.y() + other.z() * mZ.z() + other.w() * mZ.w() },
			{ other.x() * mW.x() + other.y() * mW.y() + other.z() * mW.z() + other.w() * mW.w() },
		};
	}

	const Vector3<S> operator*(const Vector3<S>& other) const
	{
		return
		{
			other.x() * mX.x() + other.y() * mY.x() + other.z() * mZ.x() + mW.x(),
			other.x() * mX.y() + other.y() * mY.y() + other.z() * mZ.y() + mW.y(),
			other.x() * mX.z() + other.y() * mY.z() + other.z() * mZ.z() + mW.z(),
		};
	}

	M& operator+=(const M& other) { *this = *this + other; return *this; }
	M& operator-=(const M& other) { *this = *this - other; return *this; }
	M& operator*=(const M& other) { *this = *this * other; return *this; }

	S& operator[](const size_t index) { assert(index < Size() && "index out of range"); return mM[index]; }
	const S& operator[](const size_t index) const { assert(index < Size() && "index out of range"); return mM[index]; }

	CommaInitializer<M, S> M::operator<<(const S& s) { return CommaInitializer<M, S>(*this, s); }

	Vector4<S>& x() { return mX; }
	Vector4<S>& y() { return mY; }
	Vector4<S>& z() { return mZ; }
	Vector4<S>& w() { return mW; }
	const Vector4<S>& x() const { return mX; }
	const Vector4<S>& y() const { return mY; }
	const Vector4<S>& z() const { return mZ; }
	const Vector4<S>& w() const { return mW; }

	S* data() { return (S*)this; }
	const S* data() const { return (S*)this; }

	Vector3<S>& translation() { return mW.v3(); }
	const Vector3<S>& translation() const { return mW.v3(); }

	M& scale(const Vector3<S>& scale)
	{
		mX.v3() *= scale;
		mY.v3() *= scale;
		mZ.v3() *= scale;
		mW.v3() *= scale;

		return *this;
	}

	M& rotate(const S angle, const Vector3<S>& axis)
	{
		const S pi = (S)3.1415926535897932384626433832795028841971693993751058209749445923;
		const S	a  = -angle * (pi / 180);
		const S	s  = sin(a);
		const S	c  = cos(a);
		const S	t  = 1 - c;
		const S	x  = axis.x();
		const S	y  = axis.y();
		const S	z  = axis.z();
		const S	tx = t * x;
		const S	ty = t * y;
		const S	tz = t * z;
		const S	sx = s * x;
		const S	sy = s * y;
		const S	sz = s * z;
		const Vector3<S> nx(tx * x + c,  tx * y - sz, tx * z + sy );
		const Vector3<S> ny(ty * x + sz, ty * y + c,  ty * z - sy );
		const Vector3<S> nz(tz * x - sy, tz * y + sx, tz * z + c  );

		mX.v3() = nx * mX.x() + ny * mX.y() + nz * mX.z();
		mY.v3() = nx * mY.x() + ny * mY.y() + nz * mY.z();
		mZ.v3() = nx * mZ.x() + ny * mZ.y() + nz * mZ.z();
		mW.v3() = nx * mW.x() + ny * mW.y() + nz * mW.z();

		return *this;
	}

	M& translate(const Vector3<S>& translation)
	{
		mX.v3() += translation * mX.w();
		mY.v3() += translation * mY.w();
		mZ.v3() += translation * mZ.w();
		mW.v3() += translation * mW.w();

		return *this;
	}

	M& invert(const M& other)
	{
		mX     = { other.x().x(), other.y().x(), other.z().x(), 0 };
		mY     = { other.x().y(), other.y().y(), other.z().y(), 0 };
		mZ     = { other.x().z(), other.y().z(), other.z().z(), 0 };
		mW.x() = -( other.w().x() * mX.x() + other.w().y() * mY.x() + other.w().z() * mZ.x() );
		mW.y() = -( other.w().x() * mX.y() + other.w().y() * mY.y() + other.w().z() * mZ.y() );
		mW.z() = -( other.w().x() * mX.z() + other.w().y() * mY.z() + other.w().z() * mZ.z() );
		mW.w() = 1;

		return *this;
	}

	M& invert()
	{
		return invert(M(*this));
	}

	M inverse()
	{
		M m;
		m.invert(*this);

		return m;
	}

	static const size_t Size() { return 16; }
	static const M Identity() { return { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } }; }

private:

	union
	{
		struct { Vector4<S> mX; Vector4<S> mY; Vector4<S> mZ; Vector4<S> mW; };
		S mM[16];
	};
};

typedef Matrix4x4<bool>   Matrix4x4b;
typedef Matrix4x4<int>    Matrix4x4i;
typedef Matrix4x4<float>  Matrix4x4f;
typedef Matrix4x4<double> Matrix4x4d;

typedef Matrix4x4<bool>   Affine3b;
typedef Matrix4x4<int>    Affine3i;
typedef Matrix4x4<float>  Affine3f;
typedef Matrix4x4<double> Affine3d;

#endif
