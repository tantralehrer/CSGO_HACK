#pragma once

#include <cmath>
#include <math.h>
#include <stdlib.h>

class QAngle;

struct Vector
{
	float X;
	float Y;
	float Z;

	Vector();
	Vector(float _X, float _Y, float _Z);
	Vector(const QAngle &angle);

	Vector operator+ (const Vector &A);
	Vector operator+ (const float A);
	Vector operator+= (const Vector &A);
	Vector operator+= (const float A);
	Vector operator- (const Vector &A);
	Vector operator- (const float A);
	Vector operator-= (const Vector &A);
	Vector operator-= (const float A);
	Vector operator* (const Vector &A);
	Vector operator* (const float A);
	Vector operator*= (const Vector &A);
	Vector operator*= (const float A);
	Vector operator/ (const Vector &A);
	Vector operator/ (const float A);
	Vector operator/= (const Vector &A);
	Vector operator/= (const float A);

	inline float Vector::Length(void) const
	{
		float root = 0.0f;

		float sqsr = X*X + Y*Y + Z*Z;

		root = sqrt(sqsr);

		return root;
	}

	inline float LengthSqr(void) const
	{
		return (X*X + Y*Y + Z*Z);
	}

	float DistToSqr(const Vector &vOther)
	{
		Vector delta;

		delta.X = X - vOther.X;
		delta.Y = Y - vOther.Y;
		delta.Z = Z - vOther.Z;

		return delta.LengthSqr();
	}

	static inline float VectorNormalize(Vector& v)
	{
		float l = v.Length();
		if (l != 0.0f)
		{
			v /= l;
		}
		else
		{
			// FIXME:
			// Just copying the existing implemenation; shouldn't res.z == 0?
			v.X = v.Y = 0.0f; v.Z = 1.0f;
		}
		return l;
	}
	//===============================================
	//static inline float VectorNormalize(float * v)
	//{
	//	return VectorNormalize(*(reinterpret_cast<Vector *>(v)));
	//}

	inline float Vector::Dot(const Vector& vOther) const
	{
		const Vector& a = *this;

		return(a.X*vOther.X + a.Y*vOther.Y + a.Z*vOther.Z);
	}

	inline float Length2D(void) const
	{
		float root = 0.0f;

		float sqst = X*X + Y*Y;

		root = sqrt(sqst);

		return root;
	}
	//===============================================
	inline float Length2DSqr(void) const
	{
		return (X*X + Y*Y);
	}

	bool operator== (const Vector &A);
	bool operator!= (const Vector &A);

	bool IsValid();
};

class __declspec(align(16)) VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {}
	inline VectorAligned(float X, float Y, float Z)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	explicit VectorAligned(const Vector &vOther)
	{
		this->X = vOther.X;
		this->Y = vOther.Y;
		this->Z = vOther.Z;
	}

	VectorAligned& operator=(const Vector &vOther)
	{
		this->X = vOther.X;
		this->Y = vOther.Y;
		this->Z = vOther.Z;
		return *this;
	}

	float w;    // this space is used anyway
};

class QAngleByValue;
class QAngle
{
public:
	// Members
	float x, y, z;

	// Construction/destruction
	QAngle(void);
	QAngle(float X, float Y, float Z);
	QAngle(const Vector& vec)
	{
		x = vec.X;
		y = vec.Y;
		z = vec.Z;
	}
	//      QAngle(RadianEuler const &angles);      // evil auto type promotion!!!

	// Allow pass-by-value
	operator QAngleByValue &() { return *((QAngleByValue *)(this)); }
	operator const QAngleByValue &() const { return *((const QAngleByValue *)(this)); }

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	void Random(float minVal, float maxVal);

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float* Base();
	float const* Base() const;

	// equality
	bool operator==(const QAngle& v) const;
	bool operator!=(const QAngle& v) const;

	// arithmetic operations
	QAngle& operator+=(const QAngle &v);
	QAngle& operator-=(const QAngle &v);
	QAngle& operator*=(float s);
	QAngle& operator/=(float s);

	// Get the vector's magnitude.
	float   Length() const;
	float   LengthSqr() const;

	// negate the QAngle components
	//void  Negate();

	// No assignment operators either...
	QAngle& operator=(const QAngle& src);

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// copy constructors

	// arithmetic operations
	QAngle  operator-(void) const;

	QAngle  operator+(const QAngle& v) const;
	QAngle  operator-(const QAngle& v) const;
	QAngle  operator*(float fl) const;
	QAngle  operator/(float fl) const;
#else

private:
	// No copy constructors allowed if we're in optimal mode
	QAngle(const QAngle& vOther);

#endif
};

//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------
inline QAngle::QAngle(void)
{
#ifdef _DEBUG
#ifdef VECTOR_PARANOIA
	// Initialize to NAN to catch errors
	x = y = z = VEC_T_NAN;
#endif
#endif
}

inline QAngle::QAngle(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------
inline void QAngle::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
}

inline void QAngle::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	z = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------
inline QAngle& QAngle::operator=(const QAngle &vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------
inline bool QAngle::operator==(const QAngle& src) const
{
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool QAngle::operator!=(const QAngle& src) const
{
	return (src.x != x) || (src.y != y) || (src.z != z);
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------
inline QAngle& QAngle::operator+=(const QAngle& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

inline QAngle& QAngle::operator-=(const QAngle& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

inline QAngle& QAngle::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

inline QAngle& QAngle::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
inline float* QAngle::Base()
{
	return (float*)this;
}

inline float const* QAngle::Base() const
{
	return (float const*)this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
inline float& QAngle::operator[](int i)
{
	return ((float*)this)[i];
}

inline float QAngle::operator[](int i) const
{
	return ((float*)this)[i];
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
inline float QAngle::Length() const
{
	return (float)std::sqrt(LengthSqr());
}


inline float QAngle::LengthSqr() const
{
	return x * x + y * y + z * z;
}


//-----------------------------------------------------------------------------
// arithmetic operations (SLOW!!)
//-----------------------------------------------------------------------------
#ifndef VECTOR_NO_SLOW_OPERATIONS

inline QAngle QAngle::operator-(void) const
{
	return QAngle(-x, -y, -z);
}

inline QAngle QAngle::operator+(const QAngle& v) const
{
	QAngle res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

inline QAngle QAngle::operator-(const QAngle& v) const
{
	QAngle res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline QAngle QAngle::operator*(float fl) const
{
	QAngle res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline QAngle QAngle::operator/(float fl) const
{
	QAngle res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline QAngle operator*(float fl, const QAngle& v)
{
	return v * fl;
}

#endif // VECTOR_NO_SLOW_OPERATIONS


//QANGLE SUBTRAC
inline void QAngleSubtract(const QAngle& a, const QAngle& b, QAngle& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

//QANGLEADD
inline void QAngleAdd(const QAngle& a, const QAngle& b, QAngle& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}
