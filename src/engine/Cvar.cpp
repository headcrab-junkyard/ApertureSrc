#include "quakedef.h"
#include "Cvar.hpp"

void CCvar::Init()
{
	//Cvar_Init(); // TODO
};

void CCvar::RegisterVariable(cvar_t *variable)
{
	Cvar_RegisterVariable(variable);
};

void CCvar::Set(const char *var_name, const char *value)
{
	Cvar_Set(var_name, value);
};

void CCvar::SetValue(const char *var_name, float value)
{
	Cvar_SetValue(var_name, value);
};

float CCvar::VariableValue(const char *var_name) const
{
	return Cvar_VariableValue(var_name);
};

const char *CCvar::VariableString(const char *var_name) const
{
	return Cvar_VariableString(var_name);
};

const char *CCvar::CompleteVariable(const char *partial) const
{
	return Cvar_CompleteVariable(partial);
};

bool CCvar::HandleCommand()
{
	return Cvar_Command();
};

void CCvar::WriteVariables(IFile *f)
{
	Cvar_WriteVariables(f);
};

cvar_t *CCvar::FindVar(const char *var_name) const
{
	return Cvar_FindVar(var_name);
};