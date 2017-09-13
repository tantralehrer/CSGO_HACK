#pragma once
#include "GameTrace.hpp"

#include "../Globals.hpp"
#include "../VirtualFunction.hpp"

class TraceEngine
{
public:

	void TraceRay(const Ray_t& a, unsigned int b, ITraceFilter* c, trace_t* d)
	{
		typedef void(__thiscall* oTraceRay)(PVOID, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		auto func = call_vfunc< oTraceRay >(this, 5);

		if (func)
			func(this, a, b, c, d);
	}
};