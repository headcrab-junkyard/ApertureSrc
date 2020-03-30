#include "quakedef.h"
#include "Progs.hpp"

void CProgs::Init()
{
	PR_Init();
};

void CProgs::LoadProgs()
{
	PR_LoadProgs();
};

void CProgs::ExecuteProgram(func_t fnum)
{
	PR_ExecuteProgram(fnum);
};

void CProgs::RunError(const char *error, ...)
{
	PR_RunError(error);
};

int CProgs::SetString(const char *s)
{
	return PR_SetString(s);
};

char *CProgs::GetString(int num)
{
	return PR_GetString(num);
};

void PR_Profile_f()
{
};