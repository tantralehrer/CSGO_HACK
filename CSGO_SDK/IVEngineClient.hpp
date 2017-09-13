#pragma once

struct Vector;
struct D3DXMATRIX;

class player_info_t
{
public:
	char unknown0[0x10];
	char name[32];
	char unknown2[0x200];

	char* get_name()
	{
		return name;
	}
};

class IVEngineClient
{
public:
	void GetScreenSize(int& width, int& height);
	bool GetPlayerInfo(int ent_num, player_info_t *pinfo);
	int GetLocalPlayer();
	float Time();
	void GetViewAngle(Vector& angles);
	void SetViewAngle(Vector& angles);
	int GetMaxClients();
	bool IsConnected();
	bool IsInGame();
	const D3DXMATRIX& WorldToScreenMatrix();
};

