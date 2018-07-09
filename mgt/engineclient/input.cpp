/// @file

#include "quakedef.h"
#include "input/IInput.hpp"

void *gpInputLib{ nullptr };
IInput *gpInput{ nullptr };

bool LoadInputModule()
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

void UnloadInputModule()
{
	if(gpInputLib)
	{
		Sys_UnloadModule(gpInputLib);
		gpInputLib = nullptr;
	};
};

void IN_Init()
{
	if(!LoadInputModule())
		gpInput = nullptr; //new CInputNull(); // TODO
	
	if(!gpInput->Init())
		return;
};

void IN_Shutdown()
{
	if(gpInput)
	{
		gpInput->Shutdown();
		gpInput = nullptr;
	};
	
	UnloadInputModule();
};