#include "Vector.hpp"

Vector::Vector()
{
	this->X = 0.0f;
	this->Y = 0.0f;
	this->Z = 0.0f;
}

Vector::Vector(float _X, float _Y, float _Z)
{
	this->X = _X;
	this->Y = _Y;
	this->Z = _Z;
}

Vector::Vector(const QAngle &angle)
{
	X = angle.x;
	Y = angle.y;
	Z = angle.z;
}

Vector Vector::operator+ (const Vector &A)
{
	return Vector(this->X + A.X, this->Y + A.Y, this->Z + A.Z);
}

Vector Vector::operator+ (const float A)
{
	return Vector(this->X + A, this->Y + A, this->Z + A);
}

Vector Vector::operator+= (const Vector &A)
{
	this->X += A.X;
	this->Y += A.Y;
	this->Z += A.Z;
	return *this;
}

Vector Vector::operator+= (const float A)
{
	this->X += A;
	this->Y += A;
	this->Z += A;
	return *this;
}

Vector Vector::operator- (const Vector &A)
{
	return Vector(this->X - A.X, this->Y - A.Y, this->Z - A.Z);
}

Vector Vector::operator- (const float A)
{
	return Vector(this->X - A, this->Y - A, this->Z - A);
}

Vector Vector::operator-= (const Vector &A)
{
	this->X -= A.X;
	this->Y -= A.Y;
	this->Z -= A.Z;
	return *this;
}

Vector Vector::operator-= (const float A)
{
	this->X -= A;
	this->Y -= A;
	this->Z -= A;
	return *this;
}

Vector Vector::operator* (const Vector &A)
{
	return Vector(this->X * A.X, this->Y * A.Y, this->Z * A.Z);
}

Vector Vector::operator* (const float A)
{
	return Vector(this->X * A, this->Y * A, this->Z * A);
}

Vector Vector::operator*= (const Vector &A)
{
	this->X *= A.X;
	this->Y *= A.Y;
	this->Z *= A.Z;
	return *this;
}

Vector Vector::operator*= (const float A)
{
	this->X *= A;
	this->Y *= A;
	this->Z *= A;
	return *this;
}

Vector Vector::operator/ (const Vector &A)
{
	return Vector(this->X / A.X, this->Y / A.Y, this->Z / A.Z);
}

Vector Vector::operator/ (const float A)
{
	return Vector(this->X / A, this->Y / A, this->Z / A);
}

Vector Vector::operator/= (const Vector &A)
{
	this->X /= A.X;
	this->Y /= A.Y;
	this->Z /= A.Z;
	return *this;
}

Vector Vector::operator/= (const float A)
{
	this->X /= A;
	this->Y /= A;
	this->Z /= A;
	return *this;
}

bool Vector::operator== (const Vector &A)
{
	if (this->X == A.X
		&& this->Y == A.Y
		&& this->Z == A.Z)
		return true;

	return false;
}

bool Vector::operator!= (const Vector &A)
{
	if (this->X != A.X
		|| this->Y != A.Y
		|| this->Z != A.Z)
		return true;

	return false;
}

bool Vector::IsValid()
{
	return *this != Vector(0, 0, 0);
}

