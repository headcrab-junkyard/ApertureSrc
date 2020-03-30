#include "quakedef.h"
#include "Cmd.hpp"

void CCmd::Init()
{
	Cmd_Init();
};

void CCmd::AddCommand(const char *cmd_name, xcommand_t function)
{
	Cmd_AddCommand(cmd_name, function);
};

bool CCmd::Exists(const char *cmd_name) const
{
	return Cmd_Exists(cmd_name);
};

const char *CCmd::CompleteCommand(const char *partial) const
{
	return Cmd_CompleteCommand(partial);
};

int CCmd::GetArgc() const
{
	return Cmd_Argc();
};

const char *CCmd::GetArgv(int arg) const
{
	return Cmd_Argv(arg);
};

const char *CCmd::GetArgs() const
{
	return Cmd_Args();
};

int CCmd::CheckParm(const char *parm) const
{
	return Cmd_CheckParm(parm);
};

void CCmd::TokenizeString(const char *text)
{
	Cmd_TokenizeString(text);
};

void CCmd::ExecuteString(const char *text, cmd_source_t src)
{
	Cmd_ExecuteString(text, src);
};

void CCmd::ForwardToServer()
{
	Cmd_ForwardToServer();
};

void CCmd::Print(const char *text)
{
	Cmd_Print(text);
};