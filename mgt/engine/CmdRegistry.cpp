/// @file

#include "quakedef.h"
#include "CmdRegistry.hpp"

EXPOSE_SINGLE_INTERFACE(CCmdRegistry, ICmdRegistry, MGT_CMDREGISTRY_INTERFACE_VERSION);

CCmdRegistry::CCmdRegistry() = default;
CCmdRegistry::~CCmdRegistry() = default;

void CCmdRegistry::Add(const char *asName, pfnCmdCallback afnCallback)
{
	Cmd_AddCommand(asName, afnCallback);
};

const char *CCmdRegistry::Complete(const char *partial)
{
	return Cmd_CompleteCommand(partial);
};