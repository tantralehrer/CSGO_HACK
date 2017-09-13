#pragma once

template< typename Function > Function call_vfunc(void* Base, unsigned long Index)
{
	unsigned long** VTablePointer = (unsigned long**)Base;

	if (VTablePointer == nullptr)
		return (Function)nullptr;

	unsigned long* VTableFunctionBase = *VTablePointer;

	if (VTableFunctionBase == nullptr)
		return (Function)nullptr;

	unsigned long* dwAddress = (unsigned long*)VTableFunctionBase[Index];

	if (dwAddress == nullptr)
		return (Function)nullptr;

	return (Function)(dwAddress);
}
