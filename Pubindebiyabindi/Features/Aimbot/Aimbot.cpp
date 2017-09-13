#include "Aimbot.hpp"

#include "../../Globals.hpp"

#include "../../CSGO_SDK/C_BaseCombatWeapon.hpp"
#include "../../CSGO_SDK/CClientClass.hpp"

#include <algorithm>
#include <limits>


void Smooth(QAngle& angle, CUserCmd* cmd)
{
	QAngle delta = angle - QAngle(cmd->viewangles.X, cmd->viewangles.Y, cmd->viewangles.Z);

	float target_yaw = angle.y;
	float view_yaw = cmd->viewangles.Y;
	float smooth_factor = 50.f;

	if (angle.y < 0)
		target_yaw = 360.f + angle.y;

	if (cmd->viewangles.Y < 0)
		view_yaw = 360.f + cmd->viewangles.Y;

	float yaw = min((float)abs(target_yaw - view_yaw), 360.f - abs(target_yaw - view_yaw));

	if (cmd->viewangles.Y > 90.f && angle.y < -90.f)
	{
		angle.x = cmd->viewangles.X + delta.x / smooth_factor * Globals::Menu::Aimbot::Smooth;
		angle.y = cmd->viewangles.Y + yaw / smooth_factor * Globals::Menu::Aimbot::Smooth;

		if (angle.y > 180.f)
			angle.y = -360.f + angle.y;

		return;
	}

	if (cmd->viewangles.Y < -90.f && angle.y > 90.f)
	{
		angle.x = cmd->viewangles.X + delta.x / smooth_factor * Globals::Menu::Aimbot::Smooth;
		angle.y = cmd->viewangles.Y - yaw / smooth_factor * Globals::Menu::Aimbot::Smooth;

		if (angle.y < -180.f)
			angle.y = 360.f + angle.y;

		return;
	}

	angle = QAngle(cmd->viewangles.X, cmd->viewangles.Y, cmd->viewangles.Z) + (delta / smooth_factor * Globals::Menu::Aimbot::Smooth);
}

void CalculateAngle(Vector& src, Vector& dst, QAngle& angles)
{
	// Angle deltas
	double delta[3] = { (src.X - dst.X), (src.Y - dst.Y), (src.Z - dst.Z) };

	// Hypotenuse
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);

	angles[0] = (float)(atan(delta[2] / hyp) * 57.295779513082f);
	angles[1] = (float)(atan(delta[1] / delta[0]) * 57.295779513082f);

	if (delta[0] >= 0.0)
		angles[1] += 180.0f;

	math::NormalizeAngles(angles);
}

double DistanceBetweenCross(int X, int Y)
{
	int Width = 0, Height = 0;

	Globals::EnginePtr->GetScreenSize(Width, Height);

	int YDist = (Y - (Height / 2));
	int XDist = (X - (Width / 2));

	auto Hyp = sqrt(pow(YDist, 2) + pow(XDist, 2));
	return Hyp;
}

IClientEntity* GetClosestEnemyScreen()
{
	auto LocalPlayer = (IClientEntity*)Globals::EntityListPtr->GetClientEntity(Globals::EnginePtr->GetLocalPlayer());
	IClientEntity* ClosestEntity = nullptr;

	if (!LocalPlayer)
		return nullptr;

	double MaxDist = 999999.0;

	for (int i = 0; i < 64; ++i)
	{
		auto entity = Globals::EntityListPtr->GetClientEntity(i);

		if (!entity
			|| entity == LocalPlayer
			|| entity->GetClientClass()->GetClassID() != CCSPlayer
			|| entity->IsDormant()
			|| !entity->IsAlive()
			|| entity->GetHealth() <= 0)
			continue;

		if (!Globals::Menu::Aimbot::Friendly && entity->GetTeamNum() == LocalPlayer->GetTeamNum())
			continue;

		if (Globals::Menu::Aimbot::OnlyVisible && !entity->IsVisible(LocalPlayer, Globals::Menu::Aimbot::AimAt))
			continue;

		Vector e_vecHead;
		Vector e_vecHeadScreen;

		if (!entity->GetBonePosition(e_vecHead, Globals::Menu::Aimbot::AimAt))
			return nullptr;

		math::WorldToScreen(e_vecHead, e_vecHeadScreen);

		auto Dist = DistanceBetweenCross(e_vecHeadScreen.X, e_vecHeadScreen.Y);

		if (Dist < MaxDist && Dist <= Globals::Menu::Aimbot::ScreenFOV)
		{
			MaxDist = Dist;
			ClosestEntity = entity;
		}
	}

	return ClosestEntity;
}

IClientEntity* GetClosestEnemyAngle(CUserCmd* cmd, bool visible)
{
	auto LocalPlayer = (IClientEntity*)Globals::EntityListPtr->GetClientEntity(Globals::EnginePtr->GetLocalPlayer());
	IClientEntity* ClosestEntity = nullptr;
	float dist = 10000000.0f;

	if (!LocalPlayer)
		return nullptr;

	for (int i = 0; i < 64; ++i)
	{
		auto entity = Globals::EntityListPtr->GetClientEntity(i);

		if (!entity
			|| entity == LocalPlayer
			|| entity->GetClientClass()->GetClassID() != CCSPlayer
			|| entity->IsDormant()
			|| !entity->IsAlive()
			|| entity->GetHealth() <= 0)
			continue;

		if (!Globals::Menu::Aimbot::Friendly && entity->GetTeamNum() == LocalPlayer->GetTeamNum())
			continue;

		float e_dist = LocalPlayer->GetOrigin().DistToSqr(entity->GetOrigin());
		Vector e_vecHead;
		
		if (!entity->GetBonePosition(e_vecHead, Globals::Menu::Aimbot::AimAt))
			return nullptr;

		auto p_vecHead = LocalPlayer->GetEyePosition();
		float fov = math::GetFov(QAngle(cmd->viewangles.X, cmd->viewangles.Y, cmd->viewangles.Z), math::CalcAngle(p_vecHead, e_vecHead));

		if (visible && !entity->IsVisible(LocalPlayer, Globals::Menu::Aimbot::AimAt))
			continue;

		if (e_dist < dist && fov <= Globals::Menu::Aimbot::AimFOV)
		{
			ClosestEntity = entity;
			dist = e_dist;
		}
	}

	return ClosestEntity;
}

void AngleNormalize(float& Angle)
{
	if (Angle > 180.f)
		Angle -= 360.f;
	else if (Angle < -180.f)
		Angle += 360.f;
}

void ClampAngle(float& Angle)
{
	if (Angle > 89.f)
		Angle = 89.f;
	else if (Angle < -89.f)
		Angle = -89.f;
}

void Aimbot::CreateMove(CUserCmd *cmd)
{
	static bool IsAiming = false;
	static IClientEntity *Target = nullptr;
	bool HasTarget = false;

	if (!Globals::Menu::Aimbot::Activated)
		return;

	if (!GetAsyncKeyState(Globals::Menu::Aimbot::AimKey))
	{
		Target = nullptr;
		IsAiming = false;
		return;
	}

	auto OldAngle = QAngle(cmd->viewangles.X, cmd->viewangles.Y, cmd->viewangles.Z);
	auto OldForward = cmd->forwardmove;
	auto OldSideMove = cmd->sidemove;

	QAngle Angle = QAngle(cmd->viewangles.X, cmd->viewangles.Y, cmd->viewangles.Z);

	auto LocalPlayer = Globals::EntityListPtr->GetClientEntity(Globals::EnginePtr->GetLocalPlayer());

	if (LocalPlayer == nullptr || !LocalPlayer->IsAlive() || LocalPlayer->GetHealth() == 0)
	{
		Target = nullptr;
		IsAiming = false;
		return;
	}

	auto ActiveWeapon = (C_BaseCombatWeapon*)LocalPlayer->GetActiveWeapon();

	if (ActiveWeapon == nullptr || ActiveWeapon->IsGrenade() || ActiveWeapon->IsKnife() || ActiveWeapon->IsBomb())
	{
		Target = nullptr;
		IsAiming = false;
		return;
	}

	IClientEntity *Entity = nullptr;

	if (Globals::Menu::Aimbot::AimType == Globals::Menu::Aimbot::AIMTYPE_ANGLE)
		Entity = GetClosestEnemyAngle(cmd, Globals::Menu::Aimbot::OnlyVisible);
	else
		Entity = GetClosestEnemyScreen();

	if (Entity == nullptr)
	{
		if (Globals::Menu::Aimbot::SwitchTarget)
			IsAiming = false;

		return;
	}

	if (IsAiming && !Globals::Menu::Aimbot::SwitchTarget && Entity != Target)
		return;

	Target = Entity;

	Vector e_vecHead;
	
	if (!Target->GetBonePosition(e_vecHead, Globals::Menu::Aimbot::AimAt))
		return;

	auto p_vecHead = LocalPlayer->GetEyePosition();

	IsAiming = true;
	CalculateAngle(p_vecHead, e_vecHead, Angle);

	if (Globals::Menu::Aimbot::AimType == Globals::Menu::Aimbot::AIMTYPE_ANGLE)
	{
		auto FOV = math::GetFov(QAngle(cmd->viewangles), math::CalcAngle(p_vecHead, e_vecHead));

		if (FOV < Globals::Menu::Aimbot::AimFOV)
		{
			HasTarget = true;
			auto AngleBetween = Angle - cmd->viewangles;
			ClampAngle(AngleBetween.x);
			AngleNormalize(AngleBetween.y);

			Angle = QAngle(cmd->viewangles) + (AngleBetween / Globals::Menu::Aimbot::Smooth);
		}
	}
	else
	{
		HasTarget = true;
		auto AngleBetween = Angle - cmd->viewangles;
		ClampAngle(AngleBetween.x);
		AngleNormalize(AngleBetween.y);

		Angle = QAngle(cmd->viewangles) + (AngleBetween / Globals::Menu::Aimbot::Smooth);
	}

	if (Globals::Menu::Aimbot::RecoilControl)
	{
		if (LocalPlayer->GetShotsFired() > 1)
		{
			auto PunchAngle = LocalPlayer->GetPunchAngle() * 2.f;
			Angle = Angle - (PunchAngle / Globals::Menu::Aimbot::Smooth);
		}
	}

	math::NormalizeAngles(Angle);
	cmd->viewangles = Vector(Angle.x, Angle.y, Angle.z);

	math::CorrectMovement(OldAngle, cmd, OldForward, OldSideMove);
}

void Aimbot::Draw()
{
	if (Globals::Menu::Aimbot::DrawScreenFOV)
	{
		if (Globals::EnginePtr->IsInGame())
		{
			auto LocalPlayer = Globals::EntityListPtr->GetClientEntity(Globals::EnginePtr->GetLocalPlayer());

			if (!LocalPlayer->IsAlive() || LocalPlayer->GetHealth() == 0)
				return;

			int ScreenX, ScreenY;
			Globals::EnginePtr->GetScreenSize(ScreenX, ScreenY);
			ScreenX /= 2;
			ScreenY /= 2;

			Globals::DrawMgr->RenderCircle(0xFF000000, ScreenX, ScreenY, Globals::Menu::Aimbot::ScreenFOV);
		}
	}
}