/*TODO: 
- FIX ResetHook -> Crashing [X]
- Implement Features
*/

//System headers
#include <Windows.h>
#include <Shlwapi.h>
#include <process.h>

#include <string>
#include <fstream>
#include <experimental/filesystem>

#include <d3d9.h>
#include <d3dx9.h>

#include "../detours.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imguitablabel.h"

#include "json.hpp"

//Custom headers
#include "Globals.hpp"
#include "Pattern.hpp"

//Features
#include "Features/Visuals/Glow.hpp"
#include "Features/Visuals/ESP.hpp"
#include "Features/Misc/Bunnyhop.hpp"
#include "Features/Aimbot/Aimbot.hpp"
#include "Features/Triggerbot/Triggerbot.hpp"
#include "Features/Profiles/Profiles.hpp"
#include "Features/Misc/RecoilControl.hpp"

//Drawing
#include "Drawing/D3DFont.h"

//SDK
#include "CSGO_SDK/interface.hpp"
#include "CSGO_SDK/CUserCmd.hpp"

//Libs
#pragma comment(lib, "../detours.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "Shlwapi.lib")

/* IMGUI */
extern IMGUI_API LRESULT   ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND latestHwnd = nullptr;

/* DirectX related hooks*/
DWORD WINAPI DispatchMessageHook(MSG* msg)
{
	/*We have to grab the hwnd of csgo here because it is NULL in PresentHook*/
	//Check HWND name for CSGO
	char windowName[MAX_PATH];

	if (GetWindowTextA(msg->hwnd, windowName, MAX_PATH) != 0)
	{
		if (strcmp(windowName, "Counter-Strike: Global Offensive") == 0)
			latestHwnd = msg->hwnd;
	}
	
	ImGui_ImplDX9_WndProcHandler(msg->hwnd, msg->message, msg->wParam, msg->lParam);

	return Globals::DispatchMessageOrig(msg);
}

HRESULT WINAPI ResetHook(LPDIRECT3DDEVICE9 device, D3DPRESENT_PARAMETERS* presentationParameters)
{
	if (!Globals::Menu::Initialized)
		return Globals::ResetOrig(device, presentationParameters);

	ImGui_ImplDX9_InvalidateDeviceObjects();
	//Globals::DrawMgr->OnLostDevice();

	auto ret = Globals::ResetOrig(device, presentationParameters);

	//Globals::DrawMgr->OnResetDevice();
	ImGui_ImplDX9_CreateDeviceObjects();

	return ret;
}

HRESULT WINAPI EndSceneHook(LPDIRECT3DDEVICE9 device)
{
	return Globals::EndSceneOrig(device);
}

DWORD WINAPI PresentHook(LPDIRECT3DDEVICE9 device, const RECT* source, const RECT* dest, HWND destWnd, const RGNDATA* region)
{
	/* Initialize imgui here */
	if (!Globals::Menu::Initialized)
	{
		if (latestHwnd != nullptr)
		{
			Globals::Menu::Initialized = true;

			if (!ImGui_ImplDX9_Init(latestHwnd, device))
			{
				MessageBoxA(nullptr, "ImGui_ImplDX9_Init failed.", "OmdisCheats Error", MB_TOPMOST);
				return 0;
			}

			//Load Logo for Menu
			if (FAILED(D3DXCreateTextureFromFileW(device, (Globals::CheatPath + L"\\logo.png").c_str(), &Globals::Menu::LogoTexture)))
			{
				MessageBoxA(nullptr, "Cannot load Logo", "OmdisCheats Error", MB_TOPMOST);
				return 0;
			}

			//Skin ImGui
			ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.f, 1.f, 1.f, 0.451f));
			ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.f, 1.f, 1.f, 0.196f));
			ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.f, 1.f, 1.f, 0.588f));

			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.f, 1.f, 1.f, 0.451f));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.f, 1.f, 1.f, 0.8f));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.f, 1.f, 1.f, 0.8f));

			//Initialize DrawManager
			Globals::DrawMgr = new DrawManager(/*device*/);
		}

		Profiles::InitProfiles();
	}
	else
	{
		//Draw cursor when main window is open
		ImGui::GetIO().MouseDrawCursor = Globals::Menu::ShowMenu;

		//ImGui drawing now
		ImGui_ImplDX9_NewFrame();

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(Globals::Menu::HideMenuKey)))
			Globals::Menu::ShowMenu = !Globals::Menu::ShowMenu;

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(Globals::Menu::PanicKey)))
			Globals::Menu::PanicOn = !Globals::Menu::PanicOn;

		//Main Window
		if (!Globals::Menu::PanicOn)
		{
			if (Globals::Menu::ShowMenu)
			{
				ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiSetCond_FirstUseEver);

				if (ImGui::Begin("OmdisCheats", &Globals::Menu::ShowMenu, ImGuiWindowFlags_NoCollapse))
				{				
					static const char *TabNames[] =
					{
						"Main", "Visuals", "Aimbot", "Triggerbot", "Misc", "Profiles"
					};

					static const int NumTabs = sizeof(TabNames) / sizeof(TabNames[0]);
					static int SelectedTab = 0;

					ImGui::TabLabels(NumTabs, TabNames, SelectedTab);

					if (SelectedTab == 0)
					{
						ImGui::Image((ImTextureID)Globals::Menu::LogoTexture, ImVec2(300, 300));
						ImGui::SameLine();
						ImGui::Text("https://omdischeats.com/forum\n\n" \
							"Thanks to ocornut for his\n" \
							"awesome GUI library\n" \
							"https://github.com/ocornut/imgui\n\n" \
							"Thanks to superdoc1234 for his\n" \
							"Haze-Dumper (unknowncheats.me)\n\n" \
							"Created by Omdihar");

						const char* ImgGuiKeys_[] =
						{
							"Tab", "Left Arrow", "Right Arrow", "Up Arrow", "Down Arrow",
							"Page Up", "Page Down", "Home", "End", "Delete", "Backspace",
							"Enter", "Escape", "A", "C", "V", "X", "Y", "Z"
						};
						const char* ImgGuiKeys =
						{
							"Tab\0Left Arrow\0Right Arrow\0Up Arrow\0Down Arrow\0Page Up\0Page Down\0Home\0End\0Delete\0Backspace\0Enter\0Escape\0A\0C\0V\0X\0Y\0Z\0\0"
						};

						ImGui::Combo("Hide Key", &Globals::Menu::HideMenuKey, (const char*)ImgGuiKeys);
						ImGui::SameLine();
						ImGui::Text("(Key: %s)", ImgGuiKeys_[Globals::Menu::HideMenuKey]);

						ImGui::Combo("Panic Key", &Globals::Menu::PanicKey, (const char*)ImgGuiKeys);
						ImGui::SameLine();
						ImGui::Text("(Key: %s)", ImgGuiKeys_[Globals::Menu::PanicKey]);

#ifdef _DEBUG
						if (ImGui::CollapsingHeader("Offsets"))
						{
							ImGui::Separator();
							ImGui::Text(Globals::Offsets.dump(4).c_str());
							ImGui::Separator();
						}
#endif
					}
					else if (SelectedTab == 1/*ImGui::CollapsingHeader("Visuals")*/)
					{
						ImGui::Checkbox("Activated", &Globals::Menu::Visuals::Activated);
						ImGui::Checkbox("Only Enemy", &Globals::Menu::Visuals::OnlyEnemy);

						if (ImGui::TreeNode("ESP"))
						{
							ImGui::Checkbox("Activated", &Globals::Menu::Visuals::ESP::Activated);

							if (ImGui::TreeNode("Player"))
							{
								ImGui::Checkbox("Activated", &Globals::Menu::Visuals::ESP::Player::Activated);
								ImGui::Checkbox("Only Visible", &Globals::Menu::Visuals::ESP::Player::OnlyVisible);

								if (ImGui::TreeNode("Healthbar"))
								{
									ImGui::Checkbox("Activated", &Globals::Menu::Visuals::ESP::Player::Healthbar::Activated);
									ImGui::ColorEdit3("Color", Globals::Menu::Visuals::ESP::Player::Healthbar::Color);
									ImGui::TreePop();
								}

								if (ImGui::TreeNode("Box"))
								{
									ImGui::Checkbox("Activated", &Globals::Menu::Visuals::ESP::Player::Box::Activated);
									ImGui::ColorEdit3("Ally Color", Globals::Menu::Visuals::ESP::Player::Box::AllyColor);
									ImGui::ColorEdit3("Ally Visible Color", Globals::Menu::Visuals::ESP::Player::Box::AllyVisibleColor);

									ImGui::ColorEdit3("Enemy Color", Globals::Menu::Visuals::ESP::Player::Box::EnemyColor);
									ImGui::ColorEdit3("Enemy Visible Color", Globals::Menu::Visuals::ESP::Player::Box::EnemyVisibleColor);

									ImGui::RadioButton("2D", &Globals::Menu::Visuals::ESP::Player::Box::BoxType, 0); ImGui::SameLine();
									ImGui::RadioButton("3D", &Globals::Menu::Visuals::ESP::Player::Box::BoxType, 1);
									ImGui::TreePop();
								}

								if (ImGui::TreeNode("Name"))
								{
									ImGui::Checkbox("Activated", &Globals::Menu::Visuals::ESP::Player::Name::Activated);
									ImGui::ColorEdit3("Color", Globals::Menu::Visuals::ESP::Player::Name::Color);
									ImGui::TreePop();
								}

								if (ImGui::TreeNode("Bones"))
								{
									ImGui::Checkbox("Activated", &Globals::Menu::Visuals::ESP::Player::Bones::Activated);

									ImGui::Checkbox("Allies", &Globals::Menu::Visuals::ESP::Player::Bones::Allies);
									ImGui::Checkbox("Enemies", &Globals::Menu::Visuals::ESP::Player::Bones::Enemies);

									ImGui::ColorEdit3("AllyColor", Globals::Menu::Visuals::ESP::Player::Bones::AllyColor);
									ImGui::ColorEdit3("EnemyColor", Globals::Menu::Visuals::ESP::Player::Bones::EnemyColor);

									ImGui::TreePop();
								}

								if (ImGui::TreeNode("Lines"))
								{
									ImGui::Checkbox("Activated", &Globals::Menu::Visuals::ESP::Player::Lines::Activated);

									ImGui::Checkbox("Allies", &Globals::Menu::Visuals::ESP::Player::Lines::Allies);
									ImGui::Checkbox("Enemies", &Globals::Menu::Visuals::ESP::Player::Lines::Enemies);

									ImGui::ColorEdit3("Ally Color", Globals::Menu::Visuals::ESP::Player::Lines::AllyColor);
									ImGui::ColorEdit3("Enemy Color", Globals::Menu::Visuals::ESP::Player::Lines::EnemyColor);

									ImGui::TreePop();
								}

								if (ImGui::TreeNode("Weapon"))
								{
									ImGui::Checkbox("Activated", &Globals::Menu::Visuals::ESP::Player::Weapon::Activated);
									ImGui::ColorEdit3("Color", Globals::Menu::Visuals::ESP::Player::Weapon::Color);
									ImGui::TreePop();
								}

								ImGui::TreePop();
							}

							if (ImGui::TreeNode("Weapon"))
							{
								if (ImGui::TreeNode("Ground"))
								{
									ImGui::Checkbox("Activated", &Globals::Menu::Visuals::ESP::Weapon::Ground::Activated);
									ImGui::ColorEdit3("Color", Globals::Menu::Visuals::ESP::Weapon::Ground::Color);

									ImGui::TreePop();
								}

								ImGui::TreePop();
							}

							if (ImGui::TreeNode("Bomb"))
							{
								ImGui::Checkbox("Activated", &Globals::Menu::Visuals::ESP::Bomb::Activated);
								ImGui::ColorEdit3("Color", Globals::Menu::Visuals::ESP::Bomb::Color);
								ImGui::TreePop();
							}

							ImGui::TreePop();
						}

						if (ImGui::TreeNode("Glow"))
						{
							ImGui::Checkbox("Activated", &Globals::Menu::Visuals::Glow::Activated);
							ImGui::ColorEdit3("Team Color", Globals::Menu::Visuals::Glow::TeamColor);
							ImGui::ColorEdit3("Enemy Color", Globals::Menu::Visuals::Glow::EnemyColor);
							ImGui::TreePop();
						}

					}

					else if (SelectedTab == 2/*ImGui::CollapsingHeader("Aimbot")*/)
					{
						ImGui::Checkbox("Activated", &Globals::Menu::Aimbot::Activated);
						ImGui::Checkbox("Only Visible", &Globals::Menu::Aimbot::OnlyVisible);
						ImGui::Checkbox("Recoil Control", &Globals::Menu::Aimbot::RecoilControl);
						ImGui::Checkbox("Switch Target", &Globals::Menu::Aimbot::SwitchTarget);
						ImGui::Checkbox("Friendly", &Globals::Menu::Aimbot::Friendly);

						ImGui::Separator();

						ImGui::Text("Aim Type");
						ImGui::Indent();
						ImGui::RadioButton("Angle", (int*)&Globals::Menu::Aimbot::AimType, Globals::Menu::Aimbot::AIMTYPE_ANGLE);
						ImGui::RadioButton("Screen", (int*)&Globals::Menu::Aimbot::AimType, Globals::Menu::Aimbot::AIMTYPE_SCREEN);
						ImGui::Unindent();
						
						ImGui::Separator();

						if (Globals::Menu::Aimbot::AimType == Globals::Menu::Aimbot::AIMTYPE_ANGLE)
						{
							ImGui::SliderFloat("Angle FOV", &Globals::Menu::Aimbot::AimFOV, 1.f, 180.f);
						}
						else
						{
							ImGui::SliderInt("Screen FOV", &Globals::Menu::Aimbot::ScreenFOV, 1, 2000);
							ImGui::Checkbox("Draw FOV", &Globals::Menu::Aimbot::DrawScreenFOV);
						}

						ImGui::SliderInt("Smooth", &Globals::Menu::Aimbot::Smooth, 1, 50);

						ImGui::Separator();

						//AimKey
						static int AimKeyIds[] =
						{
							1, 2, 4, 5, 6, 16, 17, 18, 32
						};

						static const char* AimKeyList[] =
						{ 
							"Left Mouse", "Right Mouse", "Middle Mouse", "X1 Mouse", "X2 Mouse", "SHIFT",
							"CTRL", "ALT", "SPACEBAR"
						};

						static int AimKeyCurrent = std::distance(AimKeyIds, std::find(AimKeyIds, AimKeyIds + (sizeof(AimKeyIds) / sizeof(AimKeyIds[0])), Globals::Menu::Aimbot::AimKey));

						if (ImGui::ListBox("AimKey", &AimKeyCurrent, AimKeyList, sizeof(AimKeyList) / sizeof(AimKeyList[0]), 4))
						{
							Globals::Menu::Aimbot::AimKey = AimKeyIds[AimKeyCurrent];
						}

						//AimAt
						static int AimAtIds[] =
						{
							0, 1, 2, 3, 4, 5, 6
						};

						static const char* AimAtList[] =
						{
							"Pelvis", "Spine", "Spine1", "Spine2", "Spine3", "Neck",
							"Head"
						};

						static int AimAtCurrent = Globals::Menu::Aimbot::AimAt;

						ImGui::Separator();

						if (ImGui::ListBox("AimAt", &AimAtCurrent, AimAtList, sizeof(AimAtList) / sizeof(AimAtList[0])))
						{
							Globals::Menu::Aimbot::AimAt = AimAtIds[AimAtCurrent];
						}
					}

					else if (SelectedTab == 3/*ImGui::CollapsingHeader("Triggerbot")*/)
					{
						ImGui::Checkbox("Activated", &Globals::Menu::Triggerbot::Activated);

						//AimKey
						static int TriggerKeyIds[] =
						{
							1, 2, 4, 5, 6, 16, 17, 18, 32
						};

						static const char* TriggerKeyList[] =
						{
							"Left Mouse", "Right Mouse", "Middle Mouse", "X1 Mouse", "X2 Mouse", "SHIFT",
							"CTRL", "ALT", "SPACEBAR"
						};

						static int TriggerKeyCurrent = std::distance(TriggerKeyIds, std::find(TriggerKeyIds, TriggerKeyIds + (sizeof(TriggerKeyIds) / sizeof(TriggerKeyIds[0])), Globals::Menu::Triggerbot::TriggerKey));

						if (ImGui::ListBox("Trigger Key", &TriggerKeyCurrent, TriggerKeyList, sizeof(TriggerKeyList) / sizeof(TriggerKeyList[0]), 4))
						{
							Globals::Menu::Triggerbot::TriggerKey = TriggerKeyIds[TriggerKeyCurrent];
						}
					}

					else if (SelectedTab == 4/*ImGui::CollapsingHeader("Misc")*/)
					{
						ImGui::Checkbox("Bunnyhop", &Globals::Menu::Misc::Bunnyhop);
						ImGui::Checkbox("RecoilControl", &Globals::Menu::Misc::RecoilControl);
						ImGui::Checkbox("RecoilControl Crosshair", &Globals::Menu::Misc::RCSCrosshair);
					}
					else if (SelectedTab == 5)
					{
						ImGui::Text("Current Profile: %s", Profiles::CurrentProfile.c_str());
						ImGui::Separator();

						static auto ProfileGetter = [](void *Vec, int Idx, const char **OutText)
						{
							if (Idx < 0 || Idx >= Profiles::ProfileVec.size())
								return false;

							*OutText = Profiles::ProfileVec.at(Idx).c_str();

							return true;
						};

						ImGui::ListBox("Profile List", &Profiles::CurrentIndex, ProfileGetter, (void*)nullptr, Profiles::ProfileVec.size());

						ImGui::BeginGroup();
						
						if (ImGui::Button("Save"))
						{
							Profiles::DumpSettings(Profiles::ProfileVec.at(Profiles::CurrentIndex));
							Profiles::LoadSettings(Profiles::ProfileVec.at(Profiles::CurrentIndex));
							Profiles::CurrentProfile = Profiles::ProfileVec.at(Profiles::CurrentIndex);
						}

						ImGui::SameLine();
						if (ImGui::Button("New"))
							ImGui::OpenPopup("Save Modal");
						
						if (ImGui::BeginPopupModal("Save Modal"))
						{
							ImGui::Text("Please specify a name for the profile");

							static char NameBuf[32] = { 0 };
							ImGui::InputText("Name", NameBuf, sizeof(NameBuf));

							if (ImGui::Button("Save"))
							{
								//Dump Settings
								Profiles::DumpSettings(NameBuf);

								//Clear NameBuf
								memset(NameBuf, 0, sizeof(NameBuf));
								ImGui::CloseCurrentPopup();
							}

							ImGui::EndPopup();
						}

						ImGui::SameLine();
						if (ImGui::Button("Load"))
						{
							Profiles::LoadSettings(Profiles::ProfileVec.at(Profiles::CurrentIndex));
							Profiles::CurrentProfile = Profiles::ProfileVec.at(Profiles::CurrentIndex);
						}

						ImGui::SameLine();
						if (ImGui::Button("Remove") && !Profiles::ProfileVec.empty())
						{
							ImGui::OpenPopup("Remove Modal");	
						}

						if (ImGui::BeginPopupModal("Remove Modal"))
						{
							auto ProfileStr = Profiles::ProfileVec.at(Profiles::CurrentIndex);
							ImGui::Text("Are you sure you want to remove %s", ProfileStr.c_str());

							if (ImGui::Button("Remove"))
							{
								auto Path = Globals::CheatPath + L"\\Profiles\\" + std::wstring(ProfileStr.begin(), ProfileStr.end());
								_wremove(Path.c_str());
								Profiles::InitProfiles();
								Profiles::CurrentProfile = "Not set";
								ImGui::CloseCurrentPopup();
							}

							ImGui::EndPopup();
						}

						
						ImGui::EndGroup();
					}

					ImGui::End();
				}

			}

			//Begin rendering
			//Globals::DrawMgr->BeginRendering();

			////Feature implementation here
			//if (Globals::EnginePtr != nullptr)
			//{
			//	if (Globals::EnginePtr->IsInGame() && Globals::EnginePtr->IsConnected() && Globals::Menu::Visuals::Activated)
			//	{
			//		Features::Visuals::UpdateESP();
			//		Aimbot::Draw();
			//		RecoilControl::Draw();
			//	}
			//}

			//Render the GUI
			ImGui::Render();

			//End the rendering
			//Globals::DrawMgr->EndRendering();
		}
	}

	return Globals::PresentOrig(device, source, dest, destWnd, region);
}

//=======================================================

/* Ingame hooks */

bool __stdcall CreateMoveHook(float flInputSampleTime, CUserCmd* cmd)
{
	auto result = Globals::CreateMoveOrig(flInputSampleTime, cmd);

	if (cmd && cmd->command_number && !Globals::Menu::PanicOn)
	{
		//backup cmd
		CUserCmd cpy_cmd;
		memcpy(&cpy_cmd, cmd, sizeof(CUserCmd));

		if(Globals::Menu::Visuals::Activated)
			Features::Visuals::Glow::CreateMove(cmd);

		Bunnyhop::CreateMove(cmd);
		Aimbot::CreateMove(cmd);
		Triggerbot::CreateMove(cmd);
		RecoilControl::CreateMove(cmd);

		QAngle angle = cmd->viewangles;
		math::NormalizeAngles(angle);
		cmd->viewangles = Vector(angle.x, angle.y, angle.z);

		//Safety first
		if (cmd->viewangles.X > 89.0 || cmd->viewangles.X < -89.0
			|| cmd->viewangles.Y > 180.0 || cmd->viewangles.Y < -180.0
			|| cmd->viewangles.Z != 0)
		{
			MessageBoxA(nullptr, "Angles not properly set", "OmdisCheats Error", MB_TOPMOST);
			exit(-1);
			memcpy(cmd, &cpy_cmd, sizeof(CUserCmd));
		}
	}

	return result;
}

void __fastcall PaintTraverseHook(void *ThisPtr, void *EDX, unsigned int VGUIPanel, bool ForceRepaint, bool AllowForce)
{
	Globals::PaintTraverseOrig(ThisPtr, VGUIPanel, ForceRepaint, AllowForce);

	static unsigned int DrawPanel = 0;

	if (!DrawPanel)
	{
		auto PanelName = Globals::PanelPtr->GetName(VGUIPanel);

		if (strcmp(PanelName, "MatSystemTopPanel") == 0)
			DrawPanel = VGUIPanel;
	}

	if (VGUIPanel != DrawPanel)
		return;

	//Draw here
	//Feature implementation here
	if (Globals::EnginePtr != nullptr)
	{
		if (Globals::EnginePtr->IsInGame() && Globals::EnginePtr->IsConnected() && Globals::Menu::Visuals::Activated)
		{
			Features::Visuals::UpdateESP();
			Aimbot::Draw();
			RecoilControl::Draw();
		}
	}
}

//=======================================================

/* Hooker functions*/
bool HookCreateMove(HMODULE ClientModule, uint8_t *ClientPtr)
{
	//Get Input Ptr
	Globals::InputPtr = *(uint8_t**)((*(unsigned long**)ClientPtr)[15] + 0x1);

	if (Globals::InputPtr == nullptr)
		return false;

	// Hook ClientModeShared::CreateMove
	auto clientmodeshared_instr = FindPattern((unsigned long)ClientModule, 0x843000, (unsigned char*)"\xC7\x07\x00\x00\x00\x00\x85\xC9\x74\x06", "xx????xxxx");

	if (clientmodeshared_instr == 0)
		return false;

	auto clientmodeshared_vtable = *(unsigned long**)(clientmodeshared_instr + 2);

	if (clientmodeshared_vtable == nullptr)
		return false;

	Globals::CreateMoveOrig = (CreateMoveType)DetourFunction((unsigned char*)clientmodeshared_vtable[24], (unsigned char*)CreateMoveHook);

	if (Globals::CreateMoveOrig == nullptr)
		return false;

	return true;
}

bool HookPaintTraverse()
{
	if (!Globals::PanelPtr)
		return false;

	uint32_t *PanelVTable = *(uint32_t**)Globals::PanelPtr;

	if (!PanelVTable)
		return false;
	
	unsigned long OldProtect;
	VirtualProtect(&PanelVTable[41], 4, PAGE_READWRITE, &OldProtect);

	Globals::PaintTraverseOrig = (PaintTraverseType)PanelVTable[41];

	if (!Globals::PaintTraverseOrig)
		return false;

	PanelVTable[41] = (uint32_t)PaintTraverseHook;

	VirtualProtect(&PanelVTable[41], 4, OldProtect, &OldProtect);

	//initialize ESP Font
	Features::Visuals::Font = Globals::SurfacePtr->FontCreate();
	Globals::SurfacePtr->SetFontGlyphSet(Features::Visuals::Font, "Consolas", 12, 0, 0, 0, 0x200);

	return true;
}

//=======================================================

/* Initializer functions */
bool __stdcall InitDirectXHook(HMODULE module)
{
	std::cout << "Initializing DirectX Hook" << std::endl;

	latestHwnd = nullptr;

	//Verify module
	if (module == nullptr)
	{
		std::cout << "d3d9.dll is not loaded" << std::endl;
		return false;
	}

	//Retrieve virtual table of IDirect3DDevice9
	unsigned long* virtualTable = nullptr;
	auto tempTable = FindPattern((unsigned long)module, 0x128000, (unsigned char*)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");

	if (tempTable == 0)
	{
		std::cout << "IDirect3DDevice9 virtual table not found" << std::endl;
		return false;
	}

	//Copy virtual table to virtualTable
	memcpy(&virtualTable, (void*)(tempTable + 2), 4);

	//Hook IDirect3DDevice9::Present
	if (virtualTable[17] == 0)
	{
		std::cout << "IDirect3DDevice9::Present is null" << std::endl;
		return false;
	}

	Globals::PresentOrig = (PresentType)DetourFunction((BYTE*)virtualTable[17], (BYTE*)PresentHook);

	if (Globals::PresentOrig == nullptr)
	{
		std::cout << "Failed to hook IDirect3DDevice9::Present" << std::endl;
		return false;
	}

	//Hook IDirect3DDevice9::Reset
	if (virtualTable[16] == 0)
	{
		std::cout << "IDirect3DDevice9::Reset is null" << std::endl;
		return false;
	}

	Globals::ResetOrig = (ResetType)DetourFunction((BYTE*)virtualTable[16], (BYTE*)ResetHook);

	if (Globals::ResetOrig == nullptr)
	{
		std::cout << "Failed to hook IDirect3DDevice9::Reset" << std::endl;
		return false;
	}

	//Hook IDirect3DDevice9::EndScene
	if (virtualTable[42] == 0)
	{
		std::cout << "IDirect3DDevice9::EndScene is null" << std::endl;
		return false;
	}

	Globals::EndSceneOrig = (EndSceneType)DetourFunction((BYTE*)virtualTable[42], (BYTE*)EndSceneHook);

	if (Globals::EndSceneOrig == nullptr)
	{
		std::cout << "Failed to hook IDirect3DDevice9::EndScene" << std::endl;
		return false;
	}

	//Hook DispatchMessage for message handling
	Globals::DispatchMessageOrig = (DispatchMessageType)DetourFunction((BYTE*)DispatchMessageW, (BYTE*)DispatchMessageHook);

	auto CheatPathStr = std::string(Globals::CheatPath.begin(), Globals::CheatPath.end());

	//Setup ImGui
	auto IniFileName = new char[(CheatPathStr + "/imgui.ini").length() + 1];
	auto LogFileName = new char[(CheatPathStr + "/logs/imgui_log.txt").length() + 1];

	strcpy(IniFileName, (CheatPathStr + "/imgui.ini\0").c_str());
	strcpy(LogFileName, (CheatPathStr + "/logs/imgui_log.txt\0").c_str());

	std::cout << "IniFileName: " << IniFileName << std::endl;
	std::cout << "LogFileName: " << LogFileName << std::endl;

	auto& io = ImGui::GetIO();

	io.IniFilename = IniFileName;
	io.LogFilename = LogFileName;

	return true;
}

bool __stdcall InitOffsetUpdater()
{
	//Execute HazeDumper without console
	STARTUPINFOW startupInfo;
	PROCESS_INFORMATION processInfo;

	std::wstring args;

	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(STARTUPINFOW);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW;
	startupInfo.wShowWindow = SW_HIDE;

	auto OmdiPath = Globals::CheatPath.substr(0, Globals::CheatPath.length() - 12);
	auto HazeJsonPath = OmdiPath + L"\\hazedump.json";
	auto HazeDumperPath = OmdiPath + L"\\HazeDumper.bin";

	std::wcout << L"OmdiPath: " << OmdiPath << std::endl;
	std::wcout << L"HazeJsonPath: " << HazeJsonPath << std::endl;
	std::wcout << L"HazeDumperPath: " << HazeDumperPath << std::endl;

	/*MessageBoxW(nullptr, HazeJsonPath.c_str(), L"HazeJsonPath", MB_TOPMOST);
	MessageBoxW(nullptr, HazeDumperPath.c_str(), L"HazeDumper", MB_TOPMOST);*/

	//Delete previous hazedump.json
	_wremove(HazeJsonPath.c_str());

	//Do the command then terminate the command window
	args += HazeDumperPath;

	if (!CreateProcessW(args.c_str(), nullptr, nullptr, nullptr, false, CREATE_DEFAULT_ERROR_MODE,
		nullptr, OmdiPath.c_str(), &startupInfo, &processInfo))
	{
		wchar_t buf[250];
		wsprintf(buf, L"CreateProcessW failed: %i", GetLastError());
		MessageBoxW(nullptr, buf, L"OmdisCheats Error", MB_TOPMOST);

		return false;
	}


	//Wait for hazedump.json
	auto StartWait = GetTickCount64();

	while (!std::experimental::filesystem::exists(HazeJsonPath) && !std::ifstream(HazeJsonPath).good())
	{
		if (GetTickCount64() - StartWait >= 10000)
		{
			MessageBoxW(nullptr, L"HazeDumper Timeout", L"OmdisCheats Error", MB_TOPMOST);
			return false;
		}

		Sleep(100);
	}

	Sleep(1000);

	TerminateProcess(processInfo.hProcess, 0);
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);

	//Parse JSON
	std::ifstream hazeDumpFile(HazeJsonPath);

	if (!hazeDumpFile)
	{
		MessageBoxW(nullptr, _wcserror(errno), HazeJsonPath.c_str(), MB_TOPMOST);
		return false;
	}

	auto offsetDump = std::string((std::istreambuf_iterator<char>(hazeDumpFile)), std::istreambuf_iterator<char>());
	hazeDumpFile.close();

	Globals::Offsets = nlohmann::json::parse(offsetDump);

	return true;
}

bool __stdcall InitSDKPtr(HMODULE ClientModule, HMODULE EngineModule)
{
	auto ClientFactory = CaptureFactory("client.dll");
	auto EngineFactory = CaptureFactory("engine.dll");
	auto SurfaceFactory = CaptureFactory("vguimatsurface.dll");
	auto VGUIFactory = CaptureFactory("vgui2.dll");

	if (ClientFactory == nullptr || EngineFactory == nullptr || SurfaceFactory == nullptr || VGUIFactory == nullptr)
		return false;

	Globals::EnginePtr = (IVEngineClient*)CaptureInterface(EngineFactory, "VEngineClient014");
	Globals::EntityListPtr = (IClientEntityList*)CaptureInterface(ClientFactory, "VClientEntityList003");
	Globals::TraceEnginePtr = (TraceEngine*)CaptureInterface(EngineFactory, "EngineTraceClient004");
	Globals::SurfacePtr = (ISurface*)CaptureInterface(SurfaceFactory, "VGUI_Surface031");
	Globals::ModelInfo = (IVModelInfo*)CaptureInterface(EngineFactory, "VModelInfoClient004");
	Globals::ClientPtr = (uint8_t*)CaptureInterface(ClientFactory, "VClient018");
	Globals::PanelPtr = (IPanel*)CaptureInterface(VGUIFactory, "VGUI_Panel009");

	if (Globals::EnginePtr == nullptr || Globals::EntityListPtr == nullptr || Globals::TraceEnginePtr == nullptr || Globals::ModelInfo == nullptr || Globals::ClientPtr == nullptr || Globals::PanelPtr == nullptr)
		return false;

	//Retrieve GlowPtr
	auto GlowPtrAddr = FindPattern((unsigned long)ClientModule, 0x836000, (unsigned char*)"\xE8\x00\x00\x00\x00\x83\xC4\x04\xB8\x00\x00\x00\x00\xC3\xCC", "x????xxxx????xx");

	if (!GlowPtrAddr)
		return false;

	GlowPtrAddr += 9;
	Globals::GlowPtr = *(unsigned long**)GlowPtrAddr;

	//Hooks
	if (!HookCreateMove(ClientModule, Globals::ClientPtr))
		return false;

	if (!HookPaintTraverse())
		return false;

	/*while (!Globals::EnginePtr->IsInGame())
		Sleep(1);*/

	//Retrieve GlobalVarsPtr
	/*GlobalVarsBase* GlobalVarsAddr = (GlobalVarsBase*)FindPattern((DWORD)ClientModule, 0x7CE000, (BYTE*)"\xA1\x00\x00\x00\x00\x8B\x4D\xFC\x8B\x55\x08", "x????xxxxxx");

	if (GlobalVarsAddr == nullptr)
		return false;

	GlobalVarsAddr += 1;
	Globals::GlobalVarsPtr = **(GlobalVarsBase***)GlobalVarsAddr;*/
	int dwGlobalVars = Globals::Offsets["signatures"]["dwGlobalVars"];
	Globals::GlobalVarsPtr = (GlobalVarsBase*)dwGlobalVars;

	Globals::MatrixOffset = Globals::Offsets["signatures"]["dwViewMatrix"];

	if (Globals::MatrixOffset == 0)
		return false;

	//56 57 0F 57 C0 C7 05 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
	unsigned long RenderTmp = ((unsigned long)EngineModule) + 0x12A480;/* FindPattern((unsigned long)EngineModule, 0x468000, (unsigned char*)"x56/x57/x0F/x57/xC0/xC7/x05/x00/x00/x00/x00/x00/x00/x00/x00/x00/x00/x00/x00/x00/x00/x00/x00/x00", "xxxxxxx?????????????????");*/

	if (!RenderTmp)
		return false;

	Globals::Renderer = *(CRender**)(RenderTmp + 7);

	auto dwFunction = FindPattern((unsigned long)EngineModule, 0x468000, (unsigned char*)"\xA1\x00\x00\x00\x00\x83\xF8\x01\x7E\x11\x69\xC8", "x????xxxxxxx");

	if (!dwFunction)
		return false;

	Globals::MatrixOffset = *(unsigned long*)(dwFunction + 0x11);

	return true;
}

void __cdecl InitThread(void*)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	SetConsoleTitleW(L"OmdisCheats");

	//Wait for all required modules
	HMODULE engineModule = nullptr;
	HMODULE clientModule = nullptr;
	HMODULE d3d9Module = nullptr;
	HMODULE tierModule = nullptr;

	std::cout << "Waiting for: {engine.dll, client.dll, d3d9.dll, tier0.dll}";

	while (engineModule == nullptr || clientModule == nullptr || d3d9Module == nullptr || tierModule == nullptr)
	{
		if (engineModule == nullptr)
			engineModule = GetModuleHandleA("engine.dll");

		if (clientModule == nullptr)
			clientModule = GetModuleHandleA("client.dll");

		if (d3d9Module == nullptr)
			d3d9Module = GetModuleHandleA("d3d9.dll");

		if (tierModule == nullptr)
			tierModule = GetModuleHandleA("tier0.dll");

		Sleep(1000);
	}

	//Initialize all kind of stuff now
	if (!InitDirectXHook(d3d9Module))
	{
		MessageBoxA(nullptr, "InitDirectXHook failed.", "OmdisCheats Error", MB_TOPMOST);
		exit(-1);
		return;
	}

	std::cout << "Waiting 5 seconds before initializing Offset Updater..." << std::endl;
	Sleep(5000);

	if (!InitOffsetUpdater())
	{
		MessageBoxA(nullptr, "InitOffsetUpdater failed.", "OmdisCheats Error", MB_TOPMOST);
		exit(-1);
		return;
	}

	if (!InitSDKPtr(clientModule, engineModule))
	{
		MessageBoxA(nullptr, "InitSDKPtr failed.", "OmdisCheats Error", MB_TOPMOST);
		exit(-1);
		return;
	}
}

//=======================================================

void HideModule(HANDLE hModule)
{
	DWORD dwPEB_LDR_DATA = 0;
	_asm
	{
		pushad;
		pushfd;
		mov eax, fs:[30h]		   // PEB
			mov eax, [eax + 0Ch]		  // PEB->ProcessModuleInfo
			mov dwPEB_LDR_DATA, eax	 // Save ProcessModuleInfo

			InLoadOrderModuleList :
		mov esi, [eax + 0Ch]					  // ProcessModuleInfo->InLoadOrderModuleList[FORWARD]
			mov edx, [eax + 10h]					  //  ProcessModuleInfo->InLoadOrderModuleList[BACKWARD]

			LoopInLoadOrderModuleList :
			lodsd							   //  Load First Module
			mov esi, eax		    			//  ESI points to Next Module
			mov ecx, [eax + 18h]		    		//  LDR_MODULE->BaseAddress
			cmp ecx, hModule		    		//  Is it Our Module ?
			jne SkipA		    		    	//  If Not, Next Please (@f jumps to nearest Unamed Lable @@:)
			mov ebx, [eax]				  //  [FORWARD] Module 
			mov ecx, [eax + 4]    		    	//  [BACKWARD] Module
			mov[ecx], ebx				  //  Previous Module's [FORWARD] Notation, Points to us, Replace it with, Module++
			mov[ebx + 4], ecx			    //  Next Modules, [BACKWARD] Notation, Points to us, Replace it with, Module--
			jmp InMemoryOrderModuleList		//  Hidden, so Move onto Next Set
			SkipA :
		cmp edx, esi					    //  Reached End of Modules ?
			jne LoopInLoadOrderModuleList		//  If Not, Re Loop

			InMemoryOrderModuleList :
		mov eax, dwPEB_LDR_DATA		  //  PEB->ProcessModuleInfo
			mov esi, [eax + 14h]			   //  ProcessModuleInfo->InMemoryOrderModuleList[START]
			mov edx, [eax + 18h]			   //  ProcessModuleInfo->InMemoryOrderModuleList[FINISH]

			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 10h]
			cmp ecx, hModule
			jne SkipB
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InInitializationOrderModuleList
			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList

			InInitializationOrderModuleList :
		mov eax, dwPEB_LDR_DATA				    //  PEB->ProcessModuleInfo
			mov esi, [eax + 1Ch]						 //  ProcessModuleInfo->InInitializationOrderModuleList[START]
			mov edx, [eax + 20h]						 //  ProcessModuleInfo->InInitializationOrderModuleList[FINISH]

			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 08h]
			cmp ecx, hModule
			jne SkipC
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp Finished
			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList

			Finished :
		popfd;
		popad;
	}
}

std::wstring GetModuleDir(HINSTANCE instance)
{
	wchar_t buffer[MAX_PATH];
	memset(buffer, 0, MAX_PATH * 2);
	GetModuleFileNameW(instance, buffer, MAX_PATH);
	PathRemoveFileSpecW(buffer);

	return buffer;
}

BOOL WINAPI DllMain(HINSTANCE instance, unsigned long reason, void*)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
#ifdef _DEBUG
		MessageBoxA(nullptr, "Attach Debugger now!", "Debug", MB_TOPMOST);
#endif
		Globals::CheatPath = GetModuleDir(instance);
		
		DisableThreadLibraryCalls((HMODULE)instance);
		HideModule(instance);

		_beginthread(InitThread, 0, nullptr);
	}

	return true;
}