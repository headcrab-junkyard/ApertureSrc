#include "quakedef.h"
#include "Info.hpp"

const char *CInfo::GetValueForKey(const char *s, const char *key)
{
	return Info_ValueForKey(s, key);
};

void CInfo::RemoveKey(char *s, const char *key)
{
	Info_RemoveKey(s, key);
};

void CInfo::RemovePrefixedKeys(char *start, char prefix)
{
	Info_RemovePrefixedKeys(start, prefix);
};

void CInfo::SetValueForKey(char *s, const char *key, const char *value, int maxsize)
{
	Info_SetValueForKey(s, key, value, maxsize);
};

void CInfo::SetValueForStarKey(char *s, const char *key, const char *value, int maxsize)
{
	Info_SetValueForStarKey(s, key, value, maxsize);
};

void CInfo::Print(const char *s)
{
	Info_Print(s);
};