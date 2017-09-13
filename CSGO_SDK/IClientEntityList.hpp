#pragma once
#include "IClientEntity.hpp"

#include "../Globals.hpp"
#include "../VirtualFunction.hpp"

class IClientEntityList 
{
public:
	IClientEntity* GetClientEntity(int num)
	{
		typedef IClientEntity* (__thiscall* GetClientEntityType)(void*, int);
		auto func = call_vfunc<GetClientEntityType>(this, 3);

		if (func == nullptr)
			return nullptr;

		return func(this, num);
	}

	IClientEntity* GetClientEntityFromHandle(int ent_handle)
	{
		typedef IClientEntity* (__thiscall* GetClientEntityFromHandleType)(void*, int);
		auto func = call_vfunc<GetClientEntityFromHandleType>(this, 4);

		if (func == nullptr)
			return nullptr;

		return func(this, ent_handle);
	}

	int GetHighestEntityIndex()
	{
		typedef int(__thiscall* GetHighestEntityIndexType)(void*);
		auto func = call_vfunc<GetHighestEntityIndexType>(this, 6);

		if (func == nullptr)
			return 0;

		return func(this);
	}

	int GetMaxEntities()
	{
		typedef int(__thiscall* GetMaxEntitiesType)(void*);
		auto func = call_vfunc<GetMaxEntitiesType>(this, 8);

		if (func == nullptr)
			return 0;

		return func(this);
	}

	//int GetMaxClients()
	//{
	//	typedef int(__thiscall* GetMaxEntitiesType)(void*);
	//	auto func = call_vfunc<GetMaxEntitiesType>(this, 20);

	//	if (func == nullptr)
	//		return 0;

	//	return func(this);
	//}

};