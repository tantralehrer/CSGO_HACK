#pragma once

//Globals
#define M_PI 3.14159265358979323846f
#define ANG_CLIP( ang )		if( ang > 180.0f ) { ang -= 360.0f; } else if( ang <- 180.0f ) { ang += 360.0f; }
#define	GENTITYNUM_BITS		10
#define	MAX_GENTITIES		( 1 << GENTITYNUM_BITS )

struct Vector;
class QAngle;
class CUserCmd;

class math
{
public:
	static bool WorldToScreen(const Vector& point, Vector& screen);
	static bool ScreenTransform(const Vector& origin, Vector& screen);

	static void inline SinCos(float radians, float *sine, float *cosine);
	static void AngleVectors(const QAngle &angles, Vector &forward);
	static void NormalizeAngles(QAngle& viewAngle);
	static void CorrectMovement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove);
	static float GetFov(const QAngle &viewAngle, const QAngle &aimAngle);
	static void VectorAngles(const Vector &forward, QAngle &angles);
	static QAngle CalcAngle(Vector src, Vector dst);
};


struct matrix3x4_t
{
	matrix3x4_t();
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23);
	void Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin);
	matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin);
	float *operator[](int i);
	const float *operator[](int i) const;
	float *Base();
	const float *Base() const;

	float m_flMatVal[3][4];
};