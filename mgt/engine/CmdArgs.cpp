/// @file

#include "quakedef.h"
#include "CmdArgs.hpp"

CCmdArgs::CCmdArgs(const char *text)
{
	Cmd_TokenizeString(text);
};

CCmdArgs::~CCmdArgs() = default;

const char *CCmdArgs::GetByIndex(int anArg) const
{
	return Cmd_Argv(anArg);
};

int CCmdArgs::CheckParm(const char *parm) const
{
	return Cmd_CheckParm(parm);
};

const char *CCmdArgs::GetArgValue(const char *asArg) const
{
	auto nArg{Cmd_CheckParm(asArg)};
	return nArg? Cmd_Argv(nArg) : "";
};

int CCmdArgs::GetCount() const
{
	return Cmd_Argc();
};

const char *CCmdArgs::ToString() const
{
	return Cmd_Args();
};