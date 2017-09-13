#include "CRender.hpp"
#include "../VirtualFunction.hpp"

#include "Vector.hpp"

#include <d3dx9.h>

const D3DXMATRIX& CRender::WorldToScreenMatrix()
{
	typedef const D3DXMATRIX& (__thiscall* oWorldToScreenMatrix)(void*);
	auto func = call_vfunc< oWorldToScreenMatrix >(this, 14);

	return func(this);
}