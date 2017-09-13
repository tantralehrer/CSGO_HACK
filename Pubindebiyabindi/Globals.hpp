#pragma once
#include <type_traits>
#include <string>
#include <d3d9.h>

#include "json.hpp"

//SDK
#include "CSGO_SDK/IVEngineClient.hpp"
#include "CSGO_SDK/IClientEntityList.hpp"
#include "CSGO_SDK/TraceEngine.hpp"
#include "CSGO_SDK/GlobalVarsBase.hpp"
#include "CSGO_SDK/ISurface.hpp"
#include "CSGO_SDK/IVModelInfo.hpp"
#include "CSGO_SDK/CRender.hpp"
#include "CSGO_SDK/IPanel.hpp"

//Renderer
#include "Drawing/DrawManager.hpp"


//Global type definitions
using PresentType = unsigned long(__stdcall*)(LPDIRECT3DDEVICE9, const RECT*, const RECT*, HWND, const RGNDATA*);
using ResetType = HRESULT(__stdcall*)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
using EndSceneType = HRESULT(__stdcall*)(LPDIRECT3DDEVICE9);
using DispatchMessageType = unsigned long(__stdcall*)(MSG*);

using CreateMoveType = bool(__stdcall*)(float, void*);
using PaintTraverseType = void(__thiscall*)(void*, unsigned int, bool, bool);

namespace Globals
{
	extern PresentType PresentOrig;
	extern ResetType ResetOrig;
	extern EndSceneType EndSceneOrig;
	extern DispatchMessageType DispatchMessageOrig;
	extern CreateMoveType CreateMoveOrig;
	extern PaintTraverseType PaintTraverseOrig;

	extern DrawManager *DrawMgr;

	extern IVEngineClient *EnginePtr;
	extern IClientEntityList *EntityListPtr;
	extern TraceEngine *TraceEnginePtr;
	extern unsigned long *GlowPtr;
	extern GlobalVarsBase *GlobalVarsPtr;
	extern ISurface *SurfacePtr;
	extern IVModelInfo *ModelInfo;
	extern uint8_t *InputPtr;
	extern uint8_t *ClientPtr;
	extern unsigned long MatrixOffset;
	extern CRender *Renderer;
	extern IPanel *PanelPtr;

	extern nlohmann::json Offsets;

	extern std::wstring CheatPath;

	namespace Menu
	{
		extern char PROFILES_START;

		extern bool Initialized;
		extern int HideMenuKey;
		extern bool ShowMenu;
		extern LPDIRECT3DTEXTURE9 LogoTexture;
		
		extern int PanicKey;
		extern bool PanicOn;

		namespace Visuals
		{
			extern bool Activated;
			extern bool OnlyEnemy;

			namespace ESP
			{
				extern bool Activated;
				
				namespace Player
				{
					extern bool Activated;
					extern bool OnlyVisible;

					namespace Healthbar
					{
						extern bool Activated;
						extern float Color[3];
					};

					namespace Box
					{
						extern bool Activated;
						extern float AllyColor[3];
						extern float EnemyColor[3];

						extern float AllyVisibleColor[3];
						extern float EnemyVisibleColor[3];

						extern int BoxType;
					};

					namespace Name
					{
						extern bool Activated;
						extern float Color[3];
					};

					namespace Bones
					{
						extern bool Activated;

						extern bool Allies;
						extern bool Enemies;

						extern float AllyColor[3];
						extern float EnemyColor[3];
					};

					namespace Lines
					{
						extern bool Activated;

						extern bool Allies;
						extern bool Enemies;

						extern float AllyColor[3];
						extern float EnemyColor[3];
					};

					namespace Weapon
					{
						extern bool Activated;
						extern float Color[3];
					};

					namespace Bomb
					{
						extern bool Activated;
						extern float Color[3];
					};
				};

				namespace Weapon
				{
					namespace Ground
					{
						extern bool Activated;
						extern float Color[3];
					};
				};

				namespace Bomb
				{
					extern bool Activated;
					extern float Color[3];
				};
			};

			namespace Glow
			{
				extern bool Activated;
				extern float TeamColor[3];
				extern float EnemyColor[3];
			};
		};
	
		namespace Aimbot
		{
			enum eAimType
			{
				AIMTYPE_ANGLE,
				AIMTYPE_SCREEN
			};

			extern bool Activated;
			extern bool Friendly;
			extern bool OnlyVisible;
			extern bool RecoilControl;

			extern int AimKey;
			extern int AimAt;

			extern eAimType AimType;
			extern float AimFOV;
			extern int ScreenFOV;
			extern bool DrawScreenFOV;

			extern int Smooth;
			extern bool SwitchTarget;
		};

		namespace Triggerbot
		{
			extern bool Activated;
			extern int TriggerKey;
		};

		namespace Misc
		{
			extern bool Bunnyhop;
			
			extern bool RecoilControl;
			extern bool RCSCrosshair;
		};

		extern char PROFILES_END;
	};
};