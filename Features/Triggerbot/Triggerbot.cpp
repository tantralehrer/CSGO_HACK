#include "Triggerbot.hpp"

#include "../../Globals.hpp"
#include "../../CSGO_SDK/GameTrace.hpp"

#define IN_ATTACK		(1 << 0)

void Triggerbot::CreateMove(CUserCmd *cmd)
{
	if (!Globals::Menu::Triggerbot::Activated)
		return;

	if (!((1 << 16) & GetAsyncKeyState(Globals::Menu::Triggerbot::TriggerKey)))
		return;

	auto LocalPlayer = (IClientEntity*)Globals::EntityListPtr->GetClientEntity(Globals::EnginePtr->GetLocalPlayer());

	if (!LocalPlayer)
		return;

	Vector src, dst, forward;
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = LocalPlayer;

	QAngle viewangle = cmd->viewangles;

	viewangle += LocalPlayer->GetPunchAngle() * 2.f;

	math::AngleVectors(viewangle, forward);
	forward *= 8012.f;
	filter.pSkip = LocalPlayer;
	src = LocalPlayer->GetEyePosition();
	dst = src + forward;

	ray.Init(src, dst);

	Globals::TraceEnginePtr->TraceRay(ray, 0x46004003, &filter, &tr);

	if (!tr.m_pEnt)
		return;

	if (tr.m_pEnt->GetTeamNum() != 0 && tr.m_pEnt->GetTeamNum() != LocalPlayer->GetTeamNum() && tr.m_pEnt->IsAlive() && tr.m_pEnt->GetHealth() > 0)
		cmd->buttons |= IN_ATTACK;
}