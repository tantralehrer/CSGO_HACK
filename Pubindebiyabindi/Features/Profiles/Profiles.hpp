#pragma once
#include <string>
#include <vector>

namespace Profiles
{
	extern int CurrentIndex;
	extern std::string CurrentProfile;
	extern std::vector<std::string> ProfileVec;

	bool InitProfiles();
	void DumpSettings(std::string ProfileName);
	void LoadSettings(std::string ProfileName);
}