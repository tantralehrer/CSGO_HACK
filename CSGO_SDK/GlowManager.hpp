#pragma once
#include "IClientEntity.hpp"
#include "Vector.hpp"

#pragma pack(push, 1)
//struct GlowObject
//{
//	IClientEntity* entity_ptr;
//	float r;
//	float g;
//	float b;
//	float a;
//	unsigned char unk1[0x10];
//	bool render_when_occuled;
//	bool render_when_unocculed;
//	bool full_bloom;
//	unsigned char unk2[0xE]; //24
//};
struct GlowObject
{
	IClientEntity* entity_ptr;
	float r;
	float g;
	float b;
	float a;
	unsigned char unk1[0x10];
	bool render_when_occuled;
	bool render_when_unocculed;
	bool full_bloom;
	unsigned char unk2[0xE]; //24
};
#pragma pack(pop)

class GlowManager
{
public:
	int Count();
	GlowObject* GetGlowObject(int index);
};