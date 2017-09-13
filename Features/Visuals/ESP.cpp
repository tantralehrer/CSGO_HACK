#include "ESP.hpp"

#include "../../Globals.hpp"
#include "../../Drawing/D3DFont.h"

#include "../../CSGO_SDK/CClientClass.hpp"
#include "../../CSGO_SDK/C_BaseCombatWeapon.hpp"

#include "../../CSGO_SDK/IVModelInfo.hpp"

#include "../../imgui/imgui.h"

namespace Features
{
	namespace Visuals
	{
		int Font = 0;

		void DrawWeapon(IClientEntity *Entity, Vector OriginPos, Vector OriginScreenPos)
		{
			if (OriginPos.X == 0 && OriginPos.Y == 0)
				return;

			auto EntityClass = Entity->GetClientClass();

			if (EntityClass == nullptr)
				return;

			std::string EntityClassName = EntityClass->GetClassNameW();

			if (EntityClassName.empty())
				return;

			if (EntityClassName.find("CWeapon") != std::string::npos
				|| EntityClassName.find("CAK47") != std::string::npos
				|| EntityClassName.find("CDEagle") != std::string::npos)
			{
				auto NameColor = Globals::Menu::Visuals::ESP::Weapon::Ground::Color;
				Globals::DrawMgr->RenderText(Font, ImGui::ColorConvertFloat4ToU32(ImVec4(NameColor[2], NameColor[1], NameColor[0], 1.f)), OriginScreenPos.X, OriginScreenPos.Y, true,EntityClassName.c_str());
			}
		}

		/*
		Player draw stuff
		*/
		void Draw3DBox(Vector Origin, Vector ViewOffset, Color Clr, int Width = 14, int AdditionalHeight = 6)
		{
			//SIDES
			Vector a, b;
			auto ColorImgui = ImGui::ColorConvertFloat4ToU32(ImVec4(Clr[2], Clr[1], Clr[0], 1.f));

			if(!math::WorldToScreen(Origin + Vector(Width, Width, AdditionalHeight), a)
			 ||!math::WorldToScreen(ViewOffset + Vector(Width, Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);

			if(!math::WorldToScreen(Origin + Vector(-Width, Width, AdditionalHeight), a)
			|| !math::WorldToScreen(ViewOffset + Vector(-Width, Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);

			if(!math::WorldToScreen(Origin + Vector(-Width, -Width, AdditionalHeight), a)
			|| !math::WorldToScreen(ViewOffset + Vector(-Width, -Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);

			if(!math::WorldToScreen(Origin + Vector(Width, -Width, AdditionalHeight), a)
			|| !math::WorldToScreen(ViewOffset + Vector(Width, -Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);

			//TOP
			if(!math::WorldToScreen(ViewOffset + Vector(Width, Width, AdditionalHeight), a)
			|| !math::WorldToScreen(ViewOffset + Vector(Width, -Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);

			if (!math::WorldToScreen(ViewOffset + Vector(Width, Width, AdditionalHeight), a)
				|| !math::WorldToScreen(ViewOffset + Vector(-Width, Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);

			if (!math::WorldToScreen(ViewOffset + Vector(-Width, -Width, AdditionalHeight), a)
				|| !math::WorldToScreen(ViewOffset + Vector(-Width, Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);

			if (!math::WorldToScreen(ViewOffset + Vector(Width, -Width, AdditionalHeight), a)
				|| !math::WorldToScreen(ViewOffset + Vector(-Width, -Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);

			//BOTTOM
			if(!math::WorldToScreen(Origin + Vector(Width, Width, AdditionalHeight), a)
			|| !math::WorldToScreen(Origin + Vector(Width, -Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);

			if(!math::WorldToScreen(Origin + Vector(Width, Width, AdditionalHeight), a)
			|| !math::WorldToScreen(Origin + Vector(-Width, Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);

			if(!math::WorldToScreen(Origin + Vector(-Width, -Width, AdditionalHeight), a)
			|| !math::WorldToScreen(Origin + Vector(-Width, Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);

			if(!math::WorldToScreen(Origin + Vector(Width, -Width, AdditionalHeight), a)
			|| !math::WorldToScreen(Origin + Vector(-Width, -Width, AdditionalHeight), b))
				return;

			Globals::DrawMgr->RenderLine(ColorImgui, a.X, a.Y, b.X, b.Y);
		}

		void DrawPlayerBox(IClientEntity *LocalPlayer, IClientEntity *Entity)
		{
			using namespace Globals::Menu::Visuals::ESP::Player::Box;
			Vector HeadPos;
			float *Clr;

			int LocalTeam = LocalPlayer->GetTeamNum();
			int EntityTeam = Entity->GetTeamNum();

			bool IsVisible = Entity->IsVisible(LocalPlayer, ECSPlayerBones::head_0);

			if (EntityTeam != LocalTeam)
				Clr = IsVisible ? EnemyVisibleColor : EnemyColor;
			else
				Clr = IsVisible ? AllyVisibleColor : AllyColor;

			if (BoxType == 0)
			{
				auto Max = Entity->GetCollideable()->OBBMaxs();

				Vector Pos, Pos3D;
				Vector Top, Top3D;

				Pos3D = Entity->GetOrigin();
				Top3D = Pos3D + Vector(0, 0, Max.Z);

				if (!math::WorldToScreen(Pos3D, Pos) || !math::WorldToScreen(Top3D, Top))
					return;

				auto Height = (Pos.Y - Top.Y);
				auto Width = Height / 2.f;

				Globals::DrawMgr->RenderRect(ImGui::ColorConvertFloat4ToU32(ImVec4(Clr[2], Clr[1], Clr[0], 1.f)), Top.X + Width, Top.Y, Width, Height);
			}
			else if (BoxType == 1)
			{
				const Vector OriginVec = Entity->GetOrigin();
				Vector HeadBoneVec;
				
				if (!Entity->GetBonePosition(HeadBoneVec, ECSPlayerBones::head_0))
					return;

				auto ViewOffsetVec = Vector(OriginVec.X, OriginVec.Y, HeadBoneVec.Z);

				Vector LocalPlayerScreenVec;

				if (!math::WorldToScreen(OriginVec, LocalPlayerScreenVec))
					return;

				Draw3DBox(OriginVec, ViewOffsetVec, Color(Clr[0], Clr[1], Clr[2]));
			}
			
		}

		void DrawPlayerBones(IClientEntity *LocalPlayer, IClientEntity *Entity)
		{
			auto StudioModel = Globals::ModelInfo->GetStudioModel(Entity->GetModel());

			if (StudioModel == nullptr)
				return;

			static matrix3x4_t BoneToWorldOut[128];

			if (!Entity->SetupBones(BoneToWorldOut, 128, 256, 0))
				return;

			int LocalTeam = LocalPlayer->GetTeamNum();
			int EntityTeam = Entity->GetTeamNum();

			bool IsVisible = Entity->IsVisible(LocalPlayer, ECSPlayerBones::head_0);

			if ((!Globals::Menu::Visuals::ESP::Player::Bones::Allies && LocalTeam == EntityTeam)
			|| (!Globals::Menu::Visuals::ESP::Player::Bones::Enemies && LocalTeam != EntityTeam))
				return;

			float *Color = nullptr;

			if (LocalTeam != EntityTeam)
				Color = Globals::Menu::Visuals::ESP::Player::Bones::EnemyColor;
			else
				Color = Globals::Menu::Visuals::ESP::Player::Bones::AllyColor;

			for (int i = 0; i < StudioModel->numbones; ++i)
			{
				auto Bone = StudioModel->pBone(i);

				if (Bone == nullptr || !(Bone->flags & 256) || Bone->parent == -1)
					continue;

				Vector BonePos1;
				if (!math::WorldToScreen(Vector(BoneToWorldOut[i][0][3], BoneToWorldOut[i][1][3], BoneToWorldOut[i][2][3]), BonePos1))
					continue;

				Vector BonePos2;

				if (!math::WorldToScreen(Vector(BoneToWorldOut[Bone->parent][0][3], BoneToWorldOut[Bone->parent][1][3], BoneToWorldOut[Bone->parent][2][3]), BonePos2))
					continue;

				auto ColorImgui = ImGui::ColorConvertFloat4ToU32(ImVec4(Color[2], Color[1], Color[0], 1.f));
				Globals::DrawMgr->RenderLine(ColorImgui, BonePos1.X, BonePos1.Y, BonePos2.X, BonePos2.Y);
			}
		}

		void DrawPlayerHealth(IClientEntity *LocalPlayer, IClientEntity *Entity)
		{

		}

		void DrawPlayer(IClientEntity *Player, IClientEntity *LocalEntity, int Index, Vector OriginScreenPos)
		{
			if (Player == nullptr || LocalEntity == nullptr || !Player->IsAlive())
				return;

			bool IsEnemy = Player->GetTeamNum() != LocalEntity->GetTeamNum();
			player_info_t PlayerInfo;

			if (Globals::Menu::Visuals::OnlyEnemy && !IsEnemy)
				return;

			if (!Globals::EnginePtr->GetPlayerInfo(Index, &PlayerInfo))
				return;

			if (PlayerInfo.name == nullptr)
				return;

			bool IsVisible = Player->IsVisible(LocalEntity, 6);

			if (Globals::Menu::Visuals::ESP::Player::OnlyVisible && !IsVisible)
				return;
			
			//Box
			if (Globals::Menu::Visuals::ESP::Player::Box::Activated)
			{
				DrawPlayerBox(LocalEntity, Player);
			}

			//Healthbar
			if (Globals::Menu::Visuals::ESP::Player::Healthbar::Activated)
			{

			}

			//Line to Enemy
			if(Globals::Menu::Visuals::ESP::Player::Lines::Activated)
			{
				if (Globals::Menu::Visuals::ESP::Player::Lines::Enemies && IsEnemy ||
					Globals::Menu::Visuals::ESP::Player::Lines::Allies && !IsEnemy)
				{
					auto Color = Globals::Menu::Visuals::ESP::Player::Lines::EnemyColor;

					if (!IsEnemy)
						Color = Globals::Menu::Visuals::ESP::Player::Lines::AllyColor;

					Vector LocalOriginScreenPos;

					if(math::WorldToScreen(LocalEntity->GetOrigin(), LocalOriginScreenPos))
						Globals::DrawMgr->RenderLine(ImGui::ColorConvertFloat4ToU32(ImVec4(Color[2], Color[1], Color[0], 1.f)), OriginScreenPos.X, OriginScreenPos.Y, LocalOriginScreenPos.X, LocalOriginScreenPos.Y);
				}
			}

			//Name
			if (Globals::Menu::Visuals::ESP::Player::Name::Activated)
			{
				auto NameColor = Globals::Menu::Visuals::ESP::Player::Name::Color;
				Globals::DrawMgr->RenderText(Font, ImGui::ColorConvertFloat4ToU32(ImVec4(NameColor[2], NameColor[1], NameColor[0], 1.f)), OriginScreenPos.X, OriginScreenPos.Y, true, PlayerInfo.name);
			}

			//Weapon
			if (Globals::Menu::Visuals::ESP::Player::Weapon::Activated)
			{
				auto ActiveWeapon = Player->GetActiveWeapon();

				if (ActiveWeapon)
				{
					auto WeaponName = ActiveWeapon->GetPrintName();

					if (WeaponName)
					{
						Vector HeadPos;
						Vector HeadScreenPos;

						if (Player->GetBonePosition(HeadPos, 6))
						{
							if (math::WorldToScreen(HeadPos, HeadScreenPos))
							{
								auto NameColor = Globals::Menu::Visuals::ESP::Player::Weapon::Color;
								Globals::DrawMgr->RenderText(Font, ImGui::ColorConvertFloat4ToU32(ImVec4(NameColor[2], NameColor[1], NameColor[0], 1.f)), HeadScreenPos.X, HeadScreenPos.Y, true, WeaponName);
							}
						}
					}
				}
			}

			if (Globals::Menu::Visuals::ESP::Player::Bones::Activated)
			{
				DrawPlayerBones(LocalEntity, Player);
			}
		}

		void DrawBomb(IClientEntity *Bomb)
		{
			auto ColorText = Bomb->GetBombDefuser() != -1 ? Color(0, 50, 200) : Color(255, 255, 255);
			auto BombTime = Bomb->GetBombTime() /*- Globals::GlobalVarsPtr->curtime*/;

			auto Origin = Bomb->GetOrigin();
			auto ViewOffset = Vector(Origin.X, Origin.Y, Origin.Z - 4);

			std::ostringstream ss;
			ss << "C4";

			if (BombTime > 0)
				ss << "(" << BombTime << ")";

			Vector LocalPlayerScreen;

			if (!math::WorldToScreen(Origin, LocalPlayerScreen))
				return;

			Draw3DBox(Origin, ViewOffset, Color(Globals::Menu::Visuals::ESP::Bomb::Color[0], Globals::Menu::Visuals::ESP::Bomb::Color[1], Globals::Menu::Visuals::ESP::Bomb::Color[2]), 7, 4);
			Globals::DrawMgr->RenderText(Font, ImGui::ColorConvertFloat4ToU32(ImVec4(Globals::Menu::Visuals::ESP::Bomb::Color[2], Globals::Menu::Visuals::ESP::Bomb::Color[1], Globals::Menu::Visuals::ESP::Bomb::Color[0], 1.f))
			, LocalPlayerScreen.X, LocalPlayerScreen.Y, true, ss.str().c_str());
		}

		void UpdateESP()
		{
			using namespace Globals::Menu::Visuals;

			//Is ESP activated
			if (!ESP::Activated)
				return;

			//Grab local player entity
			auto LocalEntity = Globals::EntityListPtr->GetClientEntity(Globals::EnginePtr->GetLocalPlayer());

			if (LocalEntity == nullptr)
				return;

			//Loop through every entity
			for (int i = 0; i < Globals::EntityListPtr->GetHighestEntityIndex(); ++i)
			{
				//Skip Local Player
				if (i == Globals::EnginePtr->GetLocalPlayer())
					continue;

				//Grab entity 'i'
				auto CurrentEntity = Globals::EntityListPtr->GetClientEntity(i);

				if (CurrentEntity == nullptr || CurrentEntity->IsDormant())
					continue;

				//Resolve Origin
				auto OriginPos = CurrentEntity->GetOrigin();
				Vector OriginScreenPos;

				if (!math::WorldToScreen(OriginPos, OriginScreenPos))
					continue;

				//Get Entity Type
				auto Client = CurrentEntity->GetClientClass();

				//Player
				if (Client->GetClassID() == CCSPlayer)
				{
					if (CurrentEntity == LocalEntity
						|| !CurrentEntity->IsAlive()
						|| CurrentEntity->GetHealth() <= 0)
						continue;				

					if (Globals::Menu::Visuals::ESP::Player::Activated)
						DrawPlayer(CurrentEntity, LocalEntity, i, OriginScreenPos);
				}
				else if (strstr(CurrentEntity->GetClientClass()->GetClassNameW(), "CWeapon"))
				{
					if (Globals::Menu::Visuals::ESP::Weapon::Ground::Activated)
						DrawWeapon(CurrentEntity, OriginPos, OriginScreenPos);
				}
				else if (Client->GetClassID() == CPlantedC4)
				{
					if (Globals::Menu::Visuals::ESP::Bomb::Activated && !CurrentEntity->IsBombDefused())
						DrawBomb(CurrentEntity);
				}
			}

		}
	};
};