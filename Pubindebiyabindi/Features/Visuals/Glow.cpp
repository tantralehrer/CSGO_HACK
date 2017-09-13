#include "Glow.hpp"

#include "../../Globals.hpp"

#include "../../CSGO_SDK/GlowManager.hpp"

void GlowEntity(int index, float *clr)
{
	auto GlowMgr = (GlowManager*)Globals::GlowPtr;
	auto Object = GlowMgr->GetGlowObject(index);

	if (!Object)
		return;

	Object->r = clr[0];
	Object->g = clr[1];
	Object->b = clr[2];
	Object->a = 1.f;
	Object->render_when_occuled = true;
}

void Features::Visuals::Glow::CreateMove(CUserCmd *cmd)
{
	if (!Globals::Menu::Visuals::Glow::Activated)
		return;

	auto LocalPlayer = Globals::EntityListPtr->GetClientEntity(Globals::EnginePtr->GetLocalPlayer());

	if (!LocalPlayer || !Globals::GlowPtr)
		return;

	for (int i = 0; i < 64; ++i)
	{
		auto Entity = Globals::EntityListPtr->GetClientEntity(i);

		if (!Entity || Entity->GetTeamNum() == 0 || !Entity->GetHealth() || !Entity->IsAlive() | Entity->IsDormant())
			continue;

		bool IsEnemy = Entity->GetTeamNum() != LocalPlayer->GetTeamNum();

		if (Globals::Menu::Visuals::OnlyEnemy && !IsEnemy)
			continue;

		GlowEntity(Entity->GetGlowIndex(), IsEnemy ? Globals::Menu::Visuals::Glow::EnemyColor : Globals::Menu::Visuals::Glow::TeamColor);
	}
}