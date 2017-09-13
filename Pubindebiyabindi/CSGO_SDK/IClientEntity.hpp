#pragma once
#include "math.hpp"

class C_BaseCombatWeapon;
class CClientClass;
struct model_t;

enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

enum ECSPlayerBones {
	pelvis = 0,
	spine_0,
	spine_1,
	spine_2,
	spine_3,
	neck_0,
	head_0,
	clavicle_L,
	arm_upper_L,
	arm_lower_L,
	hand_L
};

class ICollideable
{
public:
	virtual void pad0();
	virtual const Vector& OBBMins() const;
	virtual const Vector& OBBMaxs() const;
};

class IClientEntity
{
public:
	Vector GetVelocity();

	MoveType_t GetMoveType();

	bool IsBombTicking();
	float GetBombTime();
	bool IsBombDefused();
	int GetBombDefuser();

	ICollideable *GetCollideable();

	bool SetupBones(matrix3x4_t*, int, int, float);
	bool GetBonePosition(Vector& hitbox, int bone);

	model_t* GetModel();

	Vector GetOrigin();

	int GetTeamNum();

	int GetFlags();

	int GetHealth();

	bool IsAlive();

	Vector GetEyePosition();

	int GetShotsFired();

	Vector GetPunchAngle();

	bool IsDormant();

	int GetCrossHairID();

	bool IsVisible(IClientEntity *from, int bone);

	C_BaseCombatWeapon* GetActiveWeapon();

	int GetOwnerEntity();

	C_BaseCombatWeapon* ToBaseCombatWeapon();

	CClientClass* GetClientClass();

	int GetGlowIndex();

	unsigned long GetTickBase();

	matrix3x4_t& GetCoordinateFrame();
};
