
#include "GlowManager.hpp"

#include <iostream>

int GlowManager::Count()
{
	return *(int*)(this + 0x4);
}

GlowObject* GlowManager::GetGlowObject(int index)
{
	auto list_ptr = *(unsigned long*)this;
	auto offset = 0x38 * index;
	auto obj = list_ptr + offset;

	return (GlowObject*)obj;
}
