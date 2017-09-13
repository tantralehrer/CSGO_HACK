#pragma once

	using create_interface_type = void*(*)(const char*, int*);

	create_interface_type CaptureFactory(char* factory_module);
	void* CaptureInterface(create_interface_type capture_ptr, char* interface_name);