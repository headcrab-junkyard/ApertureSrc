

/// @file

#include "quakedef.h"

bool NET_CompareBaseAdr(netadr_t a, netadr_t b)
{
	if(a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3])
		return true;
	return false;
};

char *NET_BaseAdrToString(netadr_t a)
{
	static char s[64];
	sprintf(s, "%i.%i.%i.%i", a.ip[0], a.ip[1], a.ip[2], a.ip[3]);
	return s;
};

