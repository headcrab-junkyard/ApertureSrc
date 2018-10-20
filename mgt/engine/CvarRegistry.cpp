/// @file

#include "quakedef.h"
#include "CvarRegistry.hpp"
#include "filesystem/IFile.hpp"

EXPOSE_SINGLE_INTERFACE(CCvarRegistry, ICvarRegistry, MGT_CVARREGISTRY_INTERFACE_VERSION);

CCvarRegistry::CCvarRegistry() = default;
CCvarRegistry::~CCvarRegistry() = default;

void CCvarRegistry::Add(const char *asName, const char *asDefValue, int anFlags, const char *asDesc)
{
	Cvar_RegisterVariable(asName, asDefValue, anFlags); // TODO: description support
};

void CCvarRegistry::WriteToFile(IFile *apFile)
{
	Cvar_WriteVariables(apFile);
};

char *CCvarRegistry::Complete(const char *partial)
{
	return Cvar_CompleteVariable(partial);
};