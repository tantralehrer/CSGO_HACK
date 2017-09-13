#include "interface.hpp"

#include <Windows.h>

create_interface_type CaptureFactory(char* factory_module)
{
	create_interface_type function_ptr = nullptr;

	auto module_handle = GetModuleHandleA(factory_module);

	if (module_handle)
		function_ptr = (create_interface_type)GetProcAddress(module_handle, "CreateInterface");

	return function_ptr;
}

void* CaptureInterface(create_interface_type capture_ptr, char* interface_name)
{
	if (capture_ptr == nullptr)
		return nullptr;

	unsigned long* ptr = nullptr;


	ptr = (unsigned long*)capture_ptr(interface_name, nullptr);
	Sleep(10);

	return ptr;
}