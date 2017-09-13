#include "../Pattern.hpp"
#include "IVEngineClient.hpp"


#include "../Globals.hpp"
#include "../VirtualFunction.hpp"

#include "Vector.hpp"

#include <d3dx9.h>

void IVEngineClient::GetScreenSize(int& width, int& height)
{
	typedef void(__thiscall* oGetScreenSize)(PVOID, int&, int&);
	auto func = call_vfunc< oGetScreenSize >(this, 5);

	if (func)
		return func(this, width, height);
}
bool IVEngineClient::GetPlayerInfo(int ent_num, player_info_t *pinfo)
{
	typedef bool(__thiscall* oGetPlayerInfo)(PVOID, int, player_info_t*);
	auto func = call_vfunc< oGetPlayerInfo >(this, 8);

	if (func == nullptr)
		return false;

	return func(this, ent_num, pinfo);
}
int IVEngineClient::GetLocalPlayer()
{
	typedef int(__thiscall* oLocal)(PVOID);
	auto func = call_vfunc< oLocal >(this, 12);

	if (func == nullptr)
		return 0;

	return func(this);
}
float IVEngineClient::Time()
{
	typedef float(__thiscall* oTime)(PVOID);
	auto func = call_vfunc< oTime >(this, 14);

	if (func == nullptr)
		return 0.f;

	return func(this);
}

void IVEngineClient::GetViewAngle(Vector& angles)
{
	typedef void(__thiscall* oGetViewAngle)(PVOID, Vector&);
	auto func = call_vfunc< oGetViewAngle >(this, 18);

	if (func)
		func(this, angles);
}

void IVEngineClient::SetViewAngle(Vector& angles)
{
	//Safety first
	if (angles.X > 89.0 || angles.X < -89.0)
	{
		MessageBoxA(nullptr, "angles.X > 89 || angles.X < -89", "SetViewAngle - avoiding untrusted", MB_TOPMOST);
		exit(0);
		ExitProcess(0);

		int* ptr = 0;
		*ptr = 0;
	}

	if (angles.Y > 180.0 || angles.Y < -180.0)
	{
		MessageBoxA(nullptr, "angles.Y > 180 || angles.Y < -180", "SetViewAngle - avoiding untrusted", MB_TOPMOST);
		exit(0);
		ExitProcess(0);

		int* ptr = 0;
		*ptr = 0;
	}

	if (angles.Z != 0)
	{
		MessageBoxA(nullptr, "angles.Z != 0", "SetViewAngle - avoiding untrusted", MB_TOPMOST);
		exit(0);
		ExitProcess(0);

		int* ptr = 0;
		*ptr = 0;
	}

	typedef void(__thiscall* oSetViewAngles)(PVOID, Vector&);
	auto func = call_vfunc< oSetViewAngles >(this, 19);

	if (func)
		func(this, angles);
}

int IVEngineClient::GetMaxClients()
{
	typedef bool(__thiscall* oGetMaxClients)(PVOID);
	auto func = call_vfunc< oGetMaxClients >(this, 20);

	if (func == nullptr)
		return 0;

	return func(this);
}
bool IVEngineClient::IsConnected()
{
	typedef bool(__thiscall* oGetScreenSize)(PVOID);
	auto func = call_vfunc< oGetScreenSize >(this, 27);

	if (func == nullptr)
		return false;

	return func(this);
}
bool IVEngineClient::IsInGame()
{
	typedef bool(__thiscall* oLocal)(PVOID);
	auto func = call_vfunc< oLocal >(this, 26);

	if (func == nullptr)
		return false;

	return func(this);
}
const D3DXMATRIX& IVEngineClient::WorldToScreenMatrix()
{
	/*unsigned long *dwVMatrix = *(unsigned long**)Globals::MatrixOffset;
	unsigned long *dwResult = dwVMatrix + 2 * 528 - 68;
	return (D3DXMATRIX&)(*(unsigned long**)dwResult);*/
	//return *(D3DXMATRIX*)Globals::MatrixOffset;
	//return Globals::Renderer->WorldToScreenMatrix();
	typedef const D3DXMATRIX& (__thiscall* oWorldToScreenMatrix)(PVOID);
	auto func = call_vfunc< oWorldToScreenMatrix >(this, 37);

	if (func == nullptr)
		return D3DXMATRIX();

	return func(this);
}