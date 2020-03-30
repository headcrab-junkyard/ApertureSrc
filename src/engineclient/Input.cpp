#include "quakedef.h"
#include "Input.hpp"

void CInput::Init()
{
	IN_Init();
};

void CInput::Shutdown()
{
	IN_Shutdown();
};

void CInput::Commands()
{
	IN_Commands();
};

void CInput::Move(usercmd_t *cmd)
{
	IN_Move(cmd);
};

#ifdef _WIN32
void CInput::ClearStates()
{
	IN_ClearStates();
};
#endif