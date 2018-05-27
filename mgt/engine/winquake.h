/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief Win32-specific header file

#pragma once

#pragma warning(disable : 4229) // mgraph gets this

#include <windows.h>

#ifndef SERVERONLY
#include <ddraw.h>
#endif

extern HINSTANCE global_hInstance;
extern int global_nCmdShow;

extern qboolean ActiveApp, Minimized;

extern qboolean WinNT;

extern qboolean winsock_lib_initialized;

extern HANDLE hinput, houtput;

int(PASCAL FAR *pWSAStartup)(WORD wVersionRequired, LPWSADATA lpWSAData);
int(PASCAL FAR *pWSACleanup)();
int(PASCAL FAR *pWSAGetLastError)();
SOCKET(PASCAL FAR *psocket)(int af, int type, int protocol);
int(PASCAL FAR *pioctlsocket)(SOCKET s, long cmd, u_long FAR *argp);
int(PASCAL FAR *psetsockopt)(SOCKET s, int level, int optname, const char FAR *optval, int optlen);
int(PASCAL FAR *precvfrom)(SOCKET s, char FAR *buf, int len, int flags, struct sockaddr FAR *from, int FAR *fromlen);
int(PASCAL FAR *psendto)(SOCKET s, const char FAR *buf, int len, int flags, const struct sockaddr FAR *to, int tolen);
int(PASCAL FAR *pclosesocket)(SOCKET s);
int(PASCAL FAR *pgethostname)(char FAR *name, int namelen);
struct hostent FAR *(PASCAL FAR *pgethostbyname)(const char FAR *name);
struct hostent FAR *(PASCAL FAR *pgethostbyaddr)(const char FAR *addr, int len, int type);
int(PASCAL FAR *pgetsockname)(SOCKET s, struct sockaddr FAR *name, int FAR *namelen);