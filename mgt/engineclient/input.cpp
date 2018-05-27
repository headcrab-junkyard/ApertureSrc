/// @file

#include "quakedef.hpp"
#include "input/IInput.hpp"

void *gpInputLib{ nullptr };
IInput *gpInput{ nullptr };

bool InitInput()
{
	//if(gpInputLib)
	//return false;

	gpInputLib = Sys_LoadModule("input");

	if(!gpInputLib)
		return false;

	auto fnInputFactory{ Sys_GetFactory(gpInputLib) };

	if(!fnInputFactory)
		return false;

	gpInput = (IInput *)fnInputFactory(MGT_INPUT_INTERFACE_VERSION, nullptr);

	if(!gpInput)
		return false;

	return true;
};

void ShutdownInput()
{
	if(gpInput)
	{
		gpInput->Shutdown();
		gpInput = nullptr;
	};

	if(gpInputLib)
	{
		Sys_UnloadModule(gpInputLib);
		gpInputLib = nullptr;
	};
};

void IN_Init()
{
	if(!InitInput())
		gpInput = new CInputNull();
};

void IN_Shutdown()
{
	ShutdownInput();
};