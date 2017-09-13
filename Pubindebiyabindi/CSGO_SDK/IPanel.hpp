#pragma once
#include "../Globals.hpp"
#include "../VirtualFunction.hpp"

class IPanel
{
public:
	const char* GetName(unsigned int panel_id)
	{
		typedef const char* (__thiscall* GetNameType)(void*, int);
		auto func = call_vfunc<GetNameType>(this, 36);

		if (func == nullptr)
			return nullptr;

		return func(this, panel_id);
	}

	void PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce = true)
	{
		typedef void(__thiscall* PaintTraverseType)(PVOID, unsigned int, bool, bool);
		auto func = call_vfunc< PaintTraverseType >(this, 41);

		if (func)
			func(this, vguiPanel, forceRepaint, allowForce);
	}

};