/// @file

#include "quakedef.h"

#include "engine/ISystem.hpp"

#include "CmdProcessor.hpp"
#include "CmdBuffer.hpp"
#include "CmdArgs.hpp"
#include "CmdRegistry.hpp"
#include "CvarRegistry.hpp"

CCmdProcessor::CCmdProcessor(ISystem *apSystem, CCmdBuffer *apBuffer)
	: mpSystem(apSystem), mpBuffer(apBuffer){}

/*
============
Cmd_ExecuteString

A complete command line has been parsed, so try to execute it
FIXME: lookupnoadd the token to speed search?
============
*/
void CCmdExecutor::ExecuteString(const char *text, cmd_source_t src)
{
	cmd_source = src;
	
	CCmdArgs Args(text);
	mpCurCmdArgs = &Args;

	// execute the command line
	
	// no tokens
	if(!Args.GetCount())
		return;

	// check functions
	for(auto cmd = cmd_functions; cmd; cmd = cmd->next)
	{
		if(!Q_strcasecmp(Args.GetByIndex(0), cmd->name))
		{
			cmd->function(Args);
			return;
		};
	};

	// check alias
	for(auto a = cmd_alias; a; a = a->next)
	{
		if(!Q_strcasecmp(Args.GetByIndex(0), a->name))
		{
			mpBuffer->InsertText(a->value);
			return;
		};
	};

	// check cvars
	if(!Cvar_Command())
		mpSystem->Printf("Unknown command \"%s\"\n", Args.GetByIndex(0));
};