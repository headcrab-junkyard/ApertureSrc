/// @file

#include "quakedef.h"

/*
===============================================================================

DEMO LOOP CONTROL

===============================================================================
*/

/*
==================
Host_Demos_f

Return to looping demos
==================
*/
void Host_Demos_f(const ICmdArgs &apArgs)
{
	if(cls.state == ca_dedicated)
		return;
	
	if(cls.demonum == -1)
		cls.demonum = 1;
	
	CL_Disconnect_f(apArgs);
	CL_NextDemo();
};

/*
==================
Host_Stopdemo_f

Return to looping demos
==================
*/
void Host_Stopdemo_f(const ICmdArgs &apArgs)
{
	if(cls.state == ca_dedicated)
		return;
	
	if(!cls.demoplayback)
		return;
	
	CL_StopPlayback();
	CL_Disconnect();
};

//=============================================================================

/*
==================
Host_InitCommands
==================
*/
void Host_InitCommands()
{
	// TODO
	
	Cmd_AddCommand("demos", Host_Demos_f);
	Cmd_AddCommand("stopdemo", Host_Stopdemo_f);
};