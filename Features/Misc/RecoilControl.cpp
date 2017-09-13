#include "RecoilControl.hpp"

#include "../../Globals.hpp"

#include "../../CSGO_SDK/GameTrace.hpp"

void RecoilControl::CreateMove(CUserCmd *cmd)
{
	if (!Globals::Menu::Misc::RecoilControl)
		return;

	auto LocalPlayer = Globals::EntityListPtr->GetClientEntity(Globals::EnginePtr->GetLocalPlayer());

	if (!LocalPlayer)
		return;

	static Vector OldPunchAngle;

	if (LocalPlayer->GetShotsFired() > 1)
	{
		auto PunchAngle = LocalPlayer->GetPunchAngle() * 2.f;
		auto Angle = cmd->viewangles - (PunchAngle - OldPunchAngle);
		cmd->viewangles = Angle;

		OldPunchAngle = PunchAngle;
	}
	else
	{
		OldPunchAngle = Vector(0, 0, 0);
	}
}

void RecoilControl::Draw()
{
	if (!Globals::Menu::Misc::RCSCrosshair)
		return;

	auto LocalPlayer = Globals::EntityListPtr->GetClientEntity(Globals::EnginePtr->GetLocalPlayer());

	if (!LocalPlayer)
		return;

	auto Velocity = LocalPlayer->GetVelocity();

	if (Velocity.Length() != 0)
		return;

	Vector Viewangles;
	Globals::EnginePtr->GetViewAngle(Viewangles);
	Viewangles += LocalPlayer->GetPunchAngle() * 2.f;

	auto Start = LocalPlayer->GetEyePosition();
	Vector End;

	math::AngleVectors(Viewangles, End);
	End += Start;

	Ray_t Ray;
	trace_t Trace;
	CTraceFilter TraceFilter;

	TraceFilter.pSkip = LocalPlayer;

	Ray.Init(Start, End);

	Globals::TraceEnginePtr->TraceRay(Ray, MASK_SHOT, &TraceFilter, &Trace);

	Vector ScreenPos;

	if (math::WorldToScreen(Trace.endpos, ScreenPos))
	{
		//Globals::DrawMgr->RenderRect(0xFFFF0000, ScreenPos.X - 3, ScreenPos.Y - 3, 10, 10);
		//Globals::DrawMgr->RenderCircle(0xFFFFFF00, ScreenPos.X - 3, ScreenPos.Y - 3, 10);
		Globals::DrawMgr->FillRect(0xFFFF0000, ScreenPos.X - 3, ScreenPos.Y - 3, 10, 10);
	}

}