#include "Profiles.hpp"

#include "../../Globals.hpp"

#include "../../CSGO_SDK/Color.hpp"

#include <fstream>
#include <boost/filesystem.hpp>

#include "../../json.hpp"

namespace Profiles
{
	int CurrentIndex = 0;
	std::string CurrentProfile = "Not set";
	std::vector<std::string> ProfileVec;

	bool InitProfiles()
	{
		//Clear Profiles
		CurrentProfile = "Not set";
		ProfileVec.clear();

		//Load all existing profiles
		auto ProfileDir = Globals::CheatPath + L"\\Profiles";

		if (!CreateDirectoryW(ProfileDir.c_str(), nullptr) &&
			GetLastError() != ERROR_ALREADY_EXISTS)
			return false;
		
		try
		{
			//Load every .profile file in Profiles folder
			boost::filesystem::recursive_directory_iterator end_itr;
			for (boost::filesystem::recursive_directory_iterator itr(std::wstring(ProfileDir.begin(), ProfileDir.end())); itr != end_itr; ++itr)
			{
				//Only .profile files
				auto wstr = itr->path().filename().native();
				auto profile_name = std::string(wstr.begin(), wstr.end());
				ProfileVec.push_back(profile_name);
			}
		}
		catch (const std::exception &e)
		{
			MessageBoxA(nullptr, e.what(), "OmdisCheats Error", MB_TOPMOST);
			return false;
		}

		return true;
	}

	void LoadColor(nlohmann::json &config, float *color)
	{
		config["r"] = color[0];
		config["g"] = color[1];
		config["b"] = color[2];
	}

	void GetColor(nlohmann::json &config, float *setting)
	{
		setting[0] = config["r"];
		setting[1] = config["g"];
		setting[2] = config["b"];
	}

	void LoadSettings(std::string ProfileName)
	{
		using namespace Globals::Menu;

		auto Path = Globals::CheatPath + L"\\Profiles\\" + std::wstring(ProfileName.begin(), ProfileName.end());

		auto File = std::fstream(Path);

		if (!File)
			return;

		auto FileStr = std::string((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());

		nlohmann::json Settings = nlohmann::json::parse(FileStr);

		try
		{
			HideMenuKey = Settings["Menu"]["HideMenuKey"];
		}
		catch(const std::exception&)
		{ }
		
		PanicKey = Settings["Menu"]["PanicKey"];
		PanicOn = Settings["Menu"]["PanicOn"];

		try
		{
			Visuals::Activated = Settings["Menu"]["Visuals"]["Activated"];
		}
		catch (const std::exception&)
		{
		}

		try
		{
			Visuals::OnlyEnemy = Settings["Menu"]["Visuals"]["OnlyEnemy"];
		}
		catch(const std::exception&)
		{}

		/*ESP*/
		Visuals::ESP::Activated = Settings["Menu"]["Visuals"]["ESP"]["Activated"];
		Visuals::ESP::Player::Activated = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Activated"];
		Visuals::ESP::Player::OnlyVisible = Settings["Menu"]["Visuals"]["ESP"]["Player"]["OnlyVisible"];

		Visuals::ESP::Player::Healthbar::Activated = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Healthbar"]["Activated"];
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Healthbar"]["Color"], Visuals::ESP::Player::Healthbar::Color);

		Visuals::ESP::Player::Box::Activated = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["Activated"];
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["AllyColor"], Visuals::ESP::Player::Box::AllyColor);
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["EnemyColor"], Visuals::ESP::Player::Box::EnemyColor);
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["AllyVisibleColor"], Visuals::ESP::Player::Box::AllyVisibleColor);
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["EnemyVisibleColor"], Visuals::ESP::Player::Box::EnemyVisibleColor);
		Visuals::ESP::Player::Box::BoxType = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["BoxType"];

		Visuals::ESP::Player::Name::Activated = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Name"]["Activated"];
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Name"]["Color"], Visuals::ESP::Player::Name::Color);

		Visuals::ESP::Player::Bones::Activated = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["Activated"];
		Visuals::ESP::Player::Bones::Allies = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["Allies"];
		Visuals::ESP::Player::Bones::Enemies = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["Enemies"];
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["AllyColor"], Visuals::ESP::Player::Bones::AllyColor);
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["EnemyColor"], Visuals::ESP::Player::Bones::EnemyColor);

		Visuals::ESP::Player::Lines::Activated = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Lines"]["Activated"];
		Visuals::ESP::Player::Lines::Allies = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Lines"]["Allies"];
		Visuals::ESP::Player::Lines::Enemies = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["Enemies"];
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Lines"]["AllyColor"], Visuals::ESP::Player::Lines::AllyColor);
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Lines"]["EnemyColor"], Visuals::ESP::Player::Lines::EnemyColor);

		Visuals::ESP::Player::Weapon::Activated = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Weapon"]["Activated"];
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Weapon"]["Color"], Visuals::ESP::Player::Weapon::Color);

		Visuals::ESP::Player::Bomb::Activated = Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bomb"]["Activated"];
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bomb"]["Color"], Visuals::ESP::Player::Bomb::Color);

		Visuals::ESP::Weapon::Ground::Activated = Settings["Menu"]["Visuals"]["ESP"]["Weapon"]["Ground"]["Activated"];
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Weapon"]["Ground"]["Color"], Visuals::ESP::Weapon::Ground::Color);

		Visuals::ESP::Bomb::Activated = Settings["Menu"]["Visuals"]["ESP"]["Bomb"]["Activated"];
		GetColor(Settings["Menu"]["Visuals"]["ESP"]["Bomb"]["Color"], Visuals::ESP::Bomb::Color);

		/*Glow*/
		Visuals::Glow::Activated = Settings["Menu"]["Visuals"]["Glow"]["Activated"];
		GetColor(Settings["Menu"]["Visuals"]["Glow"]["TeamColor"], Visuals::Glow::TeamColor);
		GetColor(Settings["Menu"]["Visuals"]["Glow"]["EnemyColor"], Visuals::Glow::EnemyColor);

		/*Aimbot*/
		Aimbot::Activated = Settings["Menu"]["Aimbot"]["Activated"];
		Aimbot::Friendly = Settings["Menu"]["Aimbot"]["Friendly"];
		Aimbot::OnlyVisible = Settings["Menu"]["Aimbot"]["OnlyVisible"];
		Aimbot::RecoilControl = Settings["Menu"]["Aimbot"]["RecoilControl"];
		Aimbot::AimKey = Settings["Menu"]["Aimbot"]["AimKey"];
		Aimbot::AimAt = Settings["Menu"]["Aimbot"]["AimAt"];
		*(int*)&Aimbot::AimType = Settings["Menu"]["Aimbot"]["AimType"];
		Aimbot::AimFOV = Settings["Menu"]["Aimbot"]["AimFOV"];
		Aimbot::ScreenFOV = Settings["Menu"]["Aimbot"]["ScreenFOV"];
		Aimbot::Smooth = Settings["Menu"]["Aimbot"]["Smooth"];
		Aimbot::SwitchTarget = Settings["Menu"]["Aimbot"]["SwitchTarget"];
		//Aimbot::DrawScreenFOV = Settings["Menu"]["Aimbot"]["DrawScreenFOV"];

		try
		{
			Aimbot::DrawScreenFOV = Settings["Menu"]["Aimbot"]["DrawScreenFOV"];
		}
		catch (const std::exception&)
		{
			//Do nothing
		}

		/*Triggerbot*/
		Triggerbot::Activated = Settings["Menu"]["Triggerbot"]["Activated"];
		Triggerbot::TriggerKey = Settings["Menu"]["Triggerbot"]["TriggerKey"];

		/*Misc*/
		Misc::Bunnyhop = Settings["Menu"]["Misc"]["Bunnyhop"];

		try
		{
			Misc::RecoilControl = Settings["Menu"]["Misc"]["RecoilControl"];
			Misc::RCSCrosshair = Settings["Menu"]["Misc"]["RCSCrosshair"];
		}
		catch(const std::exception&)
		{ }
	}

	void DumpSettings(std::string ProfileName)
	{
		using namespace Globals::Menu;

		auto Path = Globals::CheatPath + L"\\Profiles\\" + std::wstring(ProfileName.begin(), ProfileName.end());

		auto File = std::ofstream(Path, std::ofstream::trunc);

		if (!File)
			return;

		nlohmann::json Settings;

		Settings["Menu"]["HideMenuKey"] = HideMenuKey;
		Settings["Menu"]["PanicKey"] = PanicKey;
		Settings["Menu"]["PanicOn"] = PanicOn;

		Settings["Menu"]["Visuals"]["Activated"] = Visuals::Activated;
		Settings["Menu"]["Visuals"]["OnlyEnemy"] = Visuals::OnlyEnemy;

		/*ESP*/
		Settings["Menu"]["Visuals"]["ESP"]["Activated"] = Visuals::ESP::Activated;
		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Activated"] = Visuals::ESP::Player::Activated;
		Settings["Menu"]["Visuals"]["ESP"]["Player"]["OnlyVisible"] = Visuals::ESP::Player::OnlyVisible;

		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Healthbar"]["Activated"] = Visuals::ESP::Player::Healthbar::Activated;
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Healthbar"]["Color"], Visuals::ESP::Player::Healthbar::Color);

		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["Activated"] = Visuals::ESP::Player::Box::Activated;
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["AllyColor"], Visuals::ESP::Player::Box::AllyColor);
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["EnemyColor"], Visuals::ESP::Player::Box::EnemyColor);
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["AllyVisibleColor"], Visuals::ESP::Player::Box::AllyVisibleColor);
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["EnemyVisibleColor"], Visuals::ESP::Player::Box::EnemyVisibleColor);
		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Box"]["BoxType"] = Visuals::ESP::Player::Box::BoxType;

		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Name"]["Activated"] = Visuals::ESP::Player::Name::Activated;
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Name"]["Color"], Visuals::ESP::Player::Name::Color);

		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["Activated"] = Visuals::ESP::Player::Bones::Activated;
		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["Allies"] = Visuals::ESP::Player::Bones::Allies;
		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["Enemies"] = Visuals::ESP::Player::Bones::Enemies;
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["AllyColor"], Visuals::ESP::Player::Bones::AllyColor);
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["EnemyColor"], Visuals::ESP::Player::Bones::EnemyColor);

		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Lines"]["Activated"] = Visuals::ESP::Player::Lines::Activated;
		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Lines"]["Allies"] = Visuals::ESP::Player::Lines::Allies;
		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bones"]["Enemies"] = Visuals::ESP::Player::Lines::Enemies;
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Lines"]["AllyColor"], Visuals::ESP::Player::Lines::AllyColor);
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Lines"]["EnemyColor"], Visuals::ESP::Player::Lines::EnemyColor);

		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Weapon"]["Activated"] = Visuals::ESP::Player::Weapon::Activated;
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Weapon"]["Color"], Visuals::ESP::Player::Weapon::Color);

		Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bomb"]["Activated"] = Visuals::ESP::Player::Bomb::Activated;
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Player"]["Bomb"]["Color"], Visuals::ESP::Player::Bomb::Color);

		Settings["Menu"]["Visuals"]["ESP"]["Weapon"]["Ground"]["Activated"] = Visuals::ESP::Weapon::Ground::Activated;
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Weapon"]["Ground"]["Color"], Visuals::ESP::Weapon::Ground::Color);

		Settings["Menu"]["Visuals"]["ESP"]["Bomb"]["Activated"] = Visuals::ESP::Bomb::Activated;
		LoadColor(Settings["Menu"]["Visuals"]["ESP"]["Bomb"]["Color"], Visuals::ESP::Bomb::Color);

		/*Glow*/
		Settings["Menu"]["Visuals"]["Glow"]["Activated"] = Visuals::Glow::Activated;
		LoadColor(Settings["Menu"]["Visuals"]["Glow"]["TeamColor"], Visuals::Glow::TeamColor);
		LoadColor(Settings["Menu"]["Visuals"]["Glow"]["EnemyColor"], Visuals::Glow::EnemyColor);

		/*Aimbot*/
		Settings["Menu"]["Aimbot"]["Activated"] = Aimbot::Activated;
		Settings["Menu"]["Aimbot"]["Friendly"] = Aimbot::Friendly;
		Settings["Menu"]["Aimbot"]["OnlyVisible"] = Aimbot::OnlyVisible;
		Settings["Menu"]["Aimbot"]["RecoilControl"] = Aimbot::RecoilControl;
		Settings["Menu"]["Aimbot"]["AimKey"] = Aimbot::AimKey;
		Settings["Menu"]["Aimbot"]["AimAt"] = Aimbot::AimAt;
		Settings["Menu"]["Aimbot"]["AimType"] = (int)Aimbot::AimType;
		Settings["Menu"]["Aimbot"]["AimFOV"] = Aimbot::AimFOV;
		Settings["Menu"]["Aimbot"]["ScreenFOV"] = Aimbot::ScreenFOV;
		Settings["Menu"]["Aimbot"]["Smooth"] = Aimbot::Smooth;
		Settings["Menu"]["Aimbot"]["SwitchTarget"] = Aimbot::SwitchTarget;
		Settings["Menu"]["Aimbot"]["DrawScreenFOV"] = Aimbot::DrawScreenFOV;

		/*Triggerbot*/
		Settings["Menu"]["Triggerbot"]["Activated"] = Triggerbot::Activated;
		Settings["Menu"]["Triggerbot"]["TriggerKey"] = Triggerbot::TriggerKey;

		/*Misc*/
		Settings["Menu"]["Misc"]["Bunnyhop"] = Misc::Bunnyhop;
		Settings["Menu"]["Misc"]["RecoilControl"] = Misc::RecoilControl;
		Settings["Menu"]["Misc"]["RCSCrosshair"] = Misc::RCSCrosshair;

		File << Settings.dump();

		//Reload Profiles
		InitProfiles();
	}

};