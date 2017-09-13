#include "IClientEntity.hpp"

#include "GameTrace.hpp"
#include "C_BaseCombatWeapon.hpp"
#include "CClientClass.hpp"

#include "../Globals.hpp"
#include "../VirtualFunction.hpp"

#include "math.hpp"

Vector IClientEntity::GetVelocity()
{
	return *(Vector*)(this + Globals::Offsets["netvars"]["m_vecVelocity"]);
}

MoveType_t IClientEntity::GetMoveType()
{
	return *(MoveType_t*)(this + Globals::Offsets["netvars"]["m_MoveType"]);
}

bool IClientEntity::IsBombTicking()
{
	return *(bool*)(this + Globals::Offsets["netvars"]["m_bBombTicking"]);
}

float IClientEntity::GetBombTime()
{
	return *(float*)(this + Globals::Offsets["netvars"]["m_flC4Blow"]);
}

bool IClientEntity::IsBombDefused()
{
	return *(bool*)(this + Globals::Offsets["netvars"]["m_bBombDefused"]);
}

int IClientEntity::GetBombDefuser()
{
	return *(int*)(this + Globals::Offsets["netvars"]["m_hBombDefuser"]);
}

ICollideable *IClientEntity::GetCollideable()
{
	return (ICollideable*)(this + Globals::Offsets["netvars"]["m_Collision"]);
}

model_t* IClientEntity::GetModel()
{
	PVOID pRenderable = (PVOID)((PUCHAR)this + 0x4);

	typedef model_t* (__thiscall* _GetModel)(void*);
	auto func = call_vfunc<_GetModel>(pRenderable, 8);

	if (func)
		return func(pRenderable);
	else
		return nullptr;

	//return *(model_t**)((DWORD)this + 0x6C);
}

__declspec(noinline) bool IClientEntity::SetupBones(matrix3x4_t* a, int b, int c, float d)
{
	PVOID pRenderable = (PVOID)((PUCHAR)this + 0x4);

	typedef bool(__thiscall* _SetupBones)(PVOID, matrix3x4_t*, int, int, float);
	auto func = call_vfunc<_SetupBones>(pRenderable, 13);

	if (func == nullptr)
		return false;

	return func(pRenderable, a, b, c, d);

}

bool IClientEntity::IsDormant()
{
	return *(bool*)(this + 0xE9/*Globals::Offsets["netvars"]["m_bDormant"]*/);
}

Vector IClientEntity::GetOrigin()
{
	return *(Vector*)(this + Globals::Offsets["netvars"]["m_vecOrigin"]);
}

int IClientEntity::GetTeamNum()
{
	return *(int*)(this + Globals::Offsets["netvars"]["m_iTeamNum"]);
}

int IClientEntity::GetFlags()
{
	return *(int*)(this + Globals::Offsets["netvars"]["m_fFlags"]);
}

bool IClientEntity::GetBonePosition(Vector& hitbox, int bone)
{
	matrix3x4_t bone_matrix[128];

	if (Globals::EnginePtr == nullptr)
		return false;

	auto time = Globals::EnginePtr->Time();

	if (time < 0.001)
		return false;

	//MessageBoxA(nullptr, "SetupBones", "debug", MB_TOPMOST);

	if (!SetupBones(bone_matrix, 128, 0x00000100, time))
		return false;


	auto hitbox_matrix = bone_matrix[bone];
	//hitbox = Vector(hitbox_matrix[0][3], hitbox_matrix[1][3], hitbox_matrix[2][3]);
	hitbox.X = hitbox_matrix[0][3];
	hitbox.Y = hitbox_matrix[1][3];
	hitbox.Z = hitbox_matrix[2][3];

	return true;
}

int IClientEntity::GetHealth()
{
	return *(int*)(this + Globals::Offsets["netvars"]["m_iHealth"]);
}

bool IClientEntity::IsAlive()
{
	return *(char*)(this + Globals::Offsets["netvars"]["m_lifeState"]) == 0;
}

bool IClientEntity::IsVisible(IClientEntity* from, int bone)
{
	try
	{
		Ray_t ray;
		trace_t tr;

		Vector bone_pos;
		Vector head_pos;

		if (from->GetBonePosition(head_pos, 10) == false)
			return false;

		if (GetBonePosition(bone_pos, bone) == false)
			return false;

		ray.Init(head_pos, bone_pos);

		CTraceFilter trace_filter;
		trace_filter.pSkip = from;

		if (Globals::TraceEnginePtr == nullptr)
			return false;

		Globals::TraceEnginePtr->TraceRay(ray, 0x46004003/*0x4600400B*/, &trace_filter, &tr);

		return (tr.m_pEnt == this || tr.fraction > 0.97f);
	}
	catch (...)
	{
		MessageBoxA(0, "Crash prevented", "", MB_TOPMOST);
		return false;
	}
}

Vector IClientEntity::GetEyePosition()
{
	return GetOrigin() + *(Vector*)(this + Globals::Offsets["netvars"]["m_vecViewOffset"]);
}

int IClientEntity::GetShotsFired()
{
	return *(int*)(this + Globals::Offsets["netvars"]["m_iShotsFired"]);
}

Vector IClientEntity::GetPunchAngle()
{
	return *(Vector*)(this + Globals::Offsets["netvars"]["m_aimPunchAngle"]);
}

int IClientEntity::GetCrossHairID()
{
	return *(int*)(this + Globals::Offsets["netvars"]["m_iCrossHairID"]);
}

C_BaseCombatWeapon* IClientEntity::GetActiveWeapon()
{
	auto active_weapon = *(void**)(this + Globals::Offsets["netvars"]["m_hActiveWeapon"]);

	if (active_weapon == nullptr)
		return nullptr;

	if (Globals::EntityListPtr == nullptr)
		return nullptr;

	return (C_BaseCombatWeapon*)Globals::EntityListPtr->GetClientEntityFromHandle((int)active_weapon);
}

int IClientEntity::GetOwnerEntity()
{
	return *(int*)(this + Globals::Offsets["netvars"]["m_hOwnerEntity"]);
}


C_BaseCombatWeapon* IClientEntity::ToBaseCombatWeapon()
{
	return (C_BaseCombatWeapon*)this;
}


CClientClass* IClientEntity::GetClientClass()
{
	int vt = *(int*)(this + 0x8);
	int fn = *(int*)(vt + 2 * 0x4);
	return *(CClientClass**)(fn + 0x1);
}

int IClientEntity::GetGlowIndex()
{
	return *(int*)(this + Globals::Offsets["netvars"]["m_iGlowIndex"]);
}

matrix3x4_t& IClientEntity::GetCoordinateFrame()
{
	return *(matrix3x4_t*)(this + Globals::Offsets["netvars"]["m_rgflCoordinateFrame"]);
}

//unsigned long IClientEntity::GetTickBase()
//{
//	return *(unsigned long*)(this + Offsets::m_nTickBase);
//}
