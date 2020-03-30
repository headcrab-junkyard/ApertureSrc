#include "quakedef.h"
#include "Console.hpp"

void CConsole::Init()
{
	Con_Init();
};

void CConsole::DrawCharacter(int cx, int line, int num)
{
	Con_DrawCharacter(cx, line, num);
};

void CConsole::DrawConsole(int lines, bool drawinput)
{
	Con_DrawConsole(lines, drawinput);
};

void CConsole::CheckResize()
{
	Con_CheckResize();
};

void CConsole::Print(const char *txt)
{
	Con_Print(txt);
};

void CConsole::Printf(const char *fmt, ...)
{
	Con_Printf(fmt);
};

void CConsole::DPrintf(const char *fmt, ...)
{
	Con_DPrintf(fmt);
};

void CConsole::SafePrintf(const char *fmt, ...)
{
	Con_SafePrintf(fmt);
};

void CConsole::NotifyBox(const char *text)
{
	Con_NotifyBox(text);
};

void CConsole::DrawNotify()
{
	Con_DrawNotify();
};

void CConsole::ClearNotify()
{
	Con_ClearNotify();
};

void Con_Clear_f()
{
};

void Con_ToggleConsole_f()
{
};