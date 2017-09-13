#include "math.hpp"
#include "Vector.hpp"
#include "../Globals.hpp"

#include "CUserCmd.hpp"

#include <d3dx9.h>

#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h

#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.

#ifndef RAD2DEG
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif

#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif

// MOVEMENT INFO
enum
{
	PITCH = 0,	// up / down
	YAW,		// left / right
	ROLL		// fall over
};

matrix3x4_t::matrix3x4_t() {}
matrix3x4_t::matrix3x4_t(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23)
{
	m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
	m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
	m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
}
void matrix3x4_t::Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin)
{
	m_flMatVal[0][0] = xAxis.X; m_flMatVal[0][1] = yAxis.X; m_flMatVal[0][2] = zAxis.X; m_flMatVal[0][3] = vecOrigin.X;
	m_flMatVal[1][0] = xAxis.Y; m_flMatVal[1][1] = yAxis.Y; m_flMatVal[1][2] = zAxis.Y; m_flMatVal[1][3] = vecOrigin.Y;
	m_flMatVal[2][0] = xAxis.Z; m_flMatVal[2][1] = yAxis.Z; m_flMatVal[2][2] = zAxis.Z; m_flMatVal[2][3] = vecOrigin.Z;
}

matrix3x4_t::matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin)
{
	Init(xAxis, yAxis, zAxis, vecOrigin);
}
float *matrix3x4_t::operator[](int i)
{
	return m_flMatVal[i];
}
const float *matrix3x4_t::operator[](int i) const
{
	return m_flMatVal[i];
}
float *matrix3x4_t::Base()
{
	return &m_flMatVal[0][0];
}
const float *matrix3x4_t::Base() const
{
	return &m_flMatVal[0][0];
}

bool math::ScreenTransform(const Vector& point, Vector& screen)
{
	if (Globals::EnginePtr == nullptr)
		return false;

	const D3DXMATRIX& worldToScreen = Globals::EnginePtr->WorldToScreenMatrix();
	screen.X = worldToScreen.m[0][0] * point.X + worldToScreen.m[0][1] * point.Y + worldToScreen.m[0][2] * point.Z + worldToScreen.m[0][3];
	screen.Y = worldToScreen.m[1][0] * point.X + worldToScreen.m[1][1] * point.Y + worldToScreen.m[1][2] * point.Z + worldToScreen.m[1][3];

	float w;
	w = worldToScreen.m[3][0] * point.X + worldToScreen.m[3][1] * point.Y + worldToScreen.m[3][2] * point.Z + worldToScreen.m[3][3];
	screen.Z = 0.0f;

	bool behind = false;
	if (w < 0.001f)
	{
		behind = true;
		screen.X *= 100000;
		screen.Y *= 100000;
	}
	else
	{
		float invw = 1.0f / w;
		screen.X *= invw;
		screen.Y *= invw;
	}

	return behind;
}

bool math::WorldToScreen(const Vector& origin, Vector& screen)
{
	//if (Globals::EnginePtr == nullptr)
	//	return false;

	const D3DXMATRIX& worldToScreen = Globals::EnginePtr->WorldToScreenMatrix(); //getting the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen.m[3][0] * origin.X + worldToScreen.m[3][1] * origin.Y + worldToScreen.m[3][2] * origin.Z + worldToScreen.m[3][3]; //Calc the angle in comparison to the player's cam
	screen.Z = 0;
	if (w > 0.01)
	{
		float inversew = 1 / w;
		int ScreenWidth, ScreenHeight;
		Globals::EnginePtr->GetScreenSize(ScreenWidth, ScreenHeight);

		screen.X = (ScreenWidth / 2) + (0.5 * ((worldToScreen.m[0][0] * origin.X + worldToScreen.m[0][1] * origin.Y + worldToScreen.m[0][2] * origin.Z + worldToScreen.m[0][3]) * inversew) * ScreenWidth + 0.5);
		screen.Y = (ScreenHeight / 2) - (0.5 * ((worldToScreen.m[1][0] * origin.X + worldToScreen.m[1][1] * origin.Y + worldToScreen.m[1][2] * origin.Z + worldToScreen.m[1][3]) * inversew) * ScreenHeight + 0.5);
		return true;
	}
	return false;

	//bool ret = false;

	//if (!ScreenTransform(origin, screen))
	//{
	//	INT width, height;

	//	if (Globals::EnginePtr == nullptr)
	//		return false;

	//	Globals::EnginePtr->GetScreenSize(width, height);

	//	float screen_x = width / 2;
	//	float screen_y = height / 2;

	//	screen_x += 0.5f * screen.X * width/* + 0.5f*/;
	//	screen_y -= 0.5f * screen.Y * height/* + 0.5f*/;

	//	screen.X = screen_x;
	//	screen.Y = screen_y;

	//	ret = true;
	//}

	//return ret;
}

void inline math::SinCos(float radians, float *sine, float *cosine)
{
	/*register double __cosr, __sinr;
	__asm ("fsincos" : "=t" (__cosr), "=u" (__sinr) : "0" (radians));
*/
	*sine = sin(radians);
	*cosine = cos(radians);
	/**sine = __sinr;
	*cosine = __cosr;*/
}


void math::AngleVectors(const QAngle &angles, Vector& forward)
{
	float sp, sy, cp, cy;

	math::SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
	math::SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);

	forward.X = cp * cy;
	forward.Y = cp * sy;
	forward.Z = -sp;
}

void math::NormalizeAngles(QAngle& viewAngle)
{
	if (viewAngle[0] > 89.f)
		viewAngle[0] = 89.f;

	if (viewAngle[0] < -89.f)
		viewAngle[0] = -89.f;

	if (viewAngle[1] > 180.f)
		viewAngle[1] -= 360.f;

	if (viewAngle[1] < -180.f)
		viewAngle[1] += 360.f;

	viewAngle[2] = 0;
}

void math::CorrectMovement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
	// side/forward move correction
	float deltaView;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (pCmd->viewangles.Y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.Y;
	else
		f2 = pCmd->viewangles.Y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);

	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

float math::GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	Vector ang, aim;

	math::AngleVectors(viewAngle, aim);
	math::AngleVectors(aimAngle, ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

void math::VectorAngles(const Vector& forward, QAngle &angles)
{
	if (forward.Y == 0.0f && forward.X == 0.0f)
	{
		angles[0] = (forward.Z > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles[1] = 0.0f;  //yaw left/right
	}
	else
	{
		angles[0] = atan2(-forward.Z, forward.Length2D()) * -180 / M_PI;
		angles[1] = atan2(forward.Y, forward.X) * 180 / M_PI;

		if (angles[1] > 90)
			angles[1] -= 180;
		else if (angles[1] < 90)
			angles[1] += 180;
		else if (angles[1] == 90)
			angles[1] = 0;
	}

	angles[2] = 0.0f;
}

QAngle math::CalcAngle(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;

	math::VectorAngles(delta, angles);

	Vector::VectorNormalize(delta);

	return angles;
}