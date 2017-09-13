#include "Globals.hpp"

namespace Globals
{
	PresentType PresentOrig = nullptr;
	ResetType ResetOrig = nullptr;
	EndSceneType EndSceneOrig = nullptr;
	DispatchMessageType DispatchMessageOrig = nullptr;
	CreateMoveType CreateMoveOrig = nullptr;
	PaintTraverseType PaintTraverseOrig = nullptr;

	DrawManager* DrawMgr = nullptr;

	IVEngineClient *EnginePtr = nullptr;
	IClientEntityList *EntityListPtr = nullptr;
	TraceEngine *TraceEnginePtr = nullptr;
	ISurface *SurfacePtr = nullptr;
	unsigned long *GlowPtr = nullptr;
	GlobalVarsBase *GlobalVarsPtr = nullptr;
	IVModelInfo *ModelInfo = nullptr;
	uint8_t *InputPtr = nullptr;
	uint8_t *ClientPtr = nullptr;
	CRender *Renderer = (CRender*)0x10775470; //56 57 0F 57 C0 C7 ?? ?? ?? ?? ?? ?? ?? ?? ?? 51
	IPanel *PanelPtr = nullptr;

	unsigned long MatrixOffset = 0;

	nlohmann::json Offsets;

	std::wstring CheatPath = L"";

	namespace Menu
	{
		char PROFILES_START = 0xFF;

		bool Initialized = false;
		bool ShowMenu = true;
		LPDIRECT3DTEXTURE9 LogoTexture = nullptr;
		int HideMenuKey = 8; //ImGuiKey_End
		int PanicKey = 1; //ImGuiKey_LeftArrow
		bool PanicOn = false;

		namespace Visuals
		{
			bool Activated = true;
			bool OnlyEnemy = true;

			namespace ESP
			{
				bool Activated = true;

				namespace Player
				{
					bool Activated = true;
					bool OnlyVisible = false;

					namespace Healthbar
					{
						bool Activated = true;
						float Color[3] = { 0, 1, 0 };
					};

					namespace Box
					{
						bool Activated = true;
						float AllyColor[3] = { 0, 0, 1 };
						float EnemyColor[3] = { 1, 0, 0 };

						float AllyVisibleColor[3] = { 0, 1, 1 };
						float EnemyVisibleColor[3] = { 0, 1, 0 };

						int BoxType = 1;
					};

					namespace Name
					{
						bool Activated = true;
						float Color[3] = { 1, 1, 1 };
					};

					namespace Bones
					{
						bool Activated = true;

						bool Allies = true;
						bool Enemies = true;

						float AllyColor[3] = { 0, 1, 0 };
						float EnemyColor[3] = { 1, 0, 0 };
					};

					namespace Lines
					{
						bool Activated = true;

						extern bool Allies = false;
						extern bool Enemies = true;

						extern float AllyColor[3] = { 0, 0, 1 };
						extern float EnemyColor[3] = { 1, 0, 0 };
					};

					namespace Weapon
					{
						bool Activated = true;
						float Color[3] = { 1, 1, 1 };
					};

					namespace Bomb
					{
						bool Activated = true;
						float Color[3] = { 1, 0, 0 };
					};
				};

				namespace Weapon
				{
					bool Activated = true;
					float Color[3] = { 1, 1, 1 };

					namespace Ground
					{
						bool Activated = true;
						float Color[3] = { 1, 1, 1 };
					};
				};

				namespace Bomb
				{
					bool Activated = true;
					float Color[3] = { 1, 1, 0 };
				};
			};

			namespace Glow
			{
				bool Activated = false;
				float TeamColor[3] = { 0, 1, 0 };
				float EnemyColor[3] = { 1, 0, 0 };
			};
		};

		namespace Aimbot
		{
			bool Activated = true;
			bool Friendly = false;
			bool OnlyVisible = false;
			bool RecoilControl = true;
			int AimKey = 1;
			int AimAt = 6;

			eAimType AimType = AIMTYPE_SCREEN;
			float AimFOV = 180.0f;
			int ScreenFOV = 300;
			bool DrawScreenFOV = true;

			int Smooth = 5;
			bool SwitchTarget = false;
		};

		namespace Triggerbot
		{
			bool Activated = true;
			int TriggerKey = 18;
		};

		namespace Misc
		{
			bool Bunnyhop = true;
			bool RecoilControl = false;
			bool RCSCrosshair = false;
		};

		char PROFILES_END = 0xFF;
	};
};