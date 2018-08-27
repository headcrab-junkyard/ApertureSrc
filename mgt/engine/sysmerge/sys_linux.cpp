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

#include "quakedef.h"
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <errno.h>
//#include <sys/param.h>
//#include <stddef.h>

int nostdout{0};

const char *basedir{"."};
const char *cachedir{"/tmp"};

cvar_t sys_linerefresh = { "sys_linerefresh", "0" }; // set for entity display

// TODO: sun-specific fs code
#define MAX_HANDLES 10

typedef struct
{
	FILE *hFile;
	char *pMap;
	int nLen;
	int nPos;
} MEMFILE;

MEMFILE sys_handles[MAX_HANDLES];

int findhandle(void)
{
	int i;

	for(i = 1; i < MAX_HANDLES; i++)
		if(!sys_handles[i].hFile)
			return i;
	Sys_Error("out of handles");
	return -1;
}

int Sys_FileOpenRead(char *path, int *hndl)
{
	FILE *f;
	int i;

	i = findhandle();

	f = fopen(path, "rb");
	if(!f)
	{
		*hndl = -1;
		return -1;
	}
	sys_handles[i].hFile = f;
	sys_handles[i].nLen = filelength(f);
	sys_handles[i].nPos = 0;
	sys_handles[i].pMap = mmap(0, sys_handles[i].nLen, PROT_READ, MAP_SHARED, fileno(sys_handles[i].hFile), 0);
	if(!sys_handles[i].pMap || (sys_handles[i].pMap == (char *)-1))
	{
		printf("mmap %s failed!", path);
		sys_handles[i].pMap = nullptr;
	}

	*hndl = i;

	return (sys_handles[i].nLen);
}

int Sys_FileOpenWrite(char *path)
{
	FILE *f;
	int i;

	i = findhandle();

	f = fopen(path, "wb");
	if(!f)
		Sys_Error("Error opening %s: %s", path, strerror(errno));
	sys_handles[i].hFile = f;
	sys_handles[i].nLen = 0;
	sys_handles[i].nPos = 0;
	sys_handles[i].pMap = nullptr;

	return i;
}

void Sys_FileClose(int handle)
{
	if(sys_handles[handle].pMap)
		if(munmap(sys_handles[handle].pMap, sys_handles[handle].nLen) != 0)
			printf("failed to unmap handle %d\n", handle);

	fclose(sys_handles[handle].hFile);
	sys_handles[handle].hFile = nullptr;
}

void Sys_FileSeek(int handle, int position)
{
	if(sys_handles[handle].pMap)
	{
		sys_handles[handle].nPos = position;
	}
	else
		fseek(sys_handles[handle].hFile, position, SEEK_SET);
}

int Sys_FileRead(int handle, void *dest, int count)
{
	if(sys_handles[handle].pMap)
	{
		int nPos = sys_handles[handle].nPos;
		if(count < 0)
			count = 0;
		if(nPos + count > sys_handles[handle].nLen)
			count = sys_handles[handle].nLen - nPos;
		memcpy(dest, &sys_handles[handle].pMap[nPos], count);
		sys_handles[handle].nPos = nPos + count;
		return (count);
	}
	else
		return fread(dest, 1, count, sys_handles[handle].hFile);
}

int Sys_FileWrite(int handle, void *data, int count)
{
	if(sys_handles[handle].pMap)
		Sys_Error("Attempted to write to read-only file %d!\n", handle);
	return fwrite(data, 1, count, sys_handles[handle].hFile);
}

/*
===============================================================================

SYSTEM IO

===============================================================================
*/

void Sys_DebugNumber(int y, int val)
{
}

/*
void Sys_Printf (char *fmt, ...)
{
	va_list		argptr;
	char		text[1024];
	
	va_start (argptr,fmt);
	vsprintf (text,fmt,argptr);
	va_end (argptr);
	fprintf(stderr, "%s", text);
	
	Con_Print (text);
}

void Sys_Printf (char *fmt, ...)
{

    va_list     argptr;
    char        text[1024], *t_p;
    int         l, r;

	if (nostdout)
		return;

    va_start (argptr,fmt);
    vsprintf (text,fmt,argptr);
    va_end (argptr);

    l = strlen(text);
    t_p = text;

// make sure everything goes through, even though we are non-blocking
    while (l)
    {
        r = write (1, text, l);
        if (r != l)
            sleep (0);
        if (r > 0)
        {
            t_p += r;
            l -= r;
        }
    }

}
*/

void Sys_Warn(const char *warning, ...)
{
	va_list argptr;
	char string[1024];

	va_start(argptr, warning);
	vsprintf(string, warning, argptr);
	va_end(argptr);
	fprintf(stderr, "Warning: %s", string);
}

/*
============
Sys_FileTime

returns -1 if not present
============
*/
int Sys_FileTime(const char *path)
{
	struct stat buf;

	if(stat(path, &buf) == -1)
		return -1;

	return buf.st_mtime;
}

int Sys_FileOpenRead(const char *path, int *handle)
{
	int h;
	struct stat fileinfo;

	h = open(path, O_RDONLY, 0666);
	*handle = h;
	if(h == -1)
		return -1;

	if(fstat(h, &fileinfo) == -1)
		Sys_Error("Error fstating %s", path);

	return fileinfo.st_size;
}

int Sys_FileOpenWrite(const char *path)
{
	int handle;

	umask(0);

	handle = open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);

	if(handle == -1)
		Sys_Error("Error opening %s: %s", path, strerror(errno));

	return handle;
}

void Sys_DebugLog(const char *file, const char *fmt, ...)
{
	va_list argptr;
	static char data[1024];
	int fd;

	va_start(argptr, fmt);
	vsprintf(data, fmt, argptr);
	va_end(argptr);
	//    fd = open(file, O_WRONLY | O_BINARY | O_CREAT | O_APPEND, 0666);
	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	write(fd, data, strlen(data));
	close(fd);
}

void Sys_EditFile(const char *filename)
{
	char cmd[256];
	char *term;
	char *editor;

	term = getenv("TERM");
	if(term && !strcmp(term, "xterm"))
	{
		editor = getenv("VISUAL");
		if(!editor)
			editor = getenv("EDITOR");
		if(!editor)
			editor = getenv("EDIT");
		if(!editor)
			editor = "vi";
		sprintf(cmd, "xterm -e %s %s", editor, filename);
		system(cmd);
	}
}

// =======================================================================
// Sleeps for microseconds
// =======================================================================

static volatile int oktogo;

void alarm_handler(int x)
{
	oktogo = 1;
};

void Sys_LineRefresh()
{
};

void floating_point_exception_handler(int whatever)
{
	//	Sys_Warn("floating point exception\n");
	signal(SIGFPE, floating_point_exception_handler);
};

//=============================================================================

/*
int main (int c, char **v)
{
	static quakeparms_t parms;
	double		time, oldtime, newtime;
	extern int vcrFile;
	extern int recording;
	int j;

//	static char cwd[1024];

//	signal(SIGFPE, floating_point_exception_handler);
	signal(SIGFPE, SIG_IGN);

#ifdef GLQUAKE
	parms.memsize = 16*1024*1024;
#else
	parms.memsize = 8*1024*1024;
#endif

	j = COM_CheckParm("-mem");
	if (j)
		parms.memsize = (int) (Q_atof(com_argv[j+1]) * 1024 * 1024);
	parms.membase = malloc (parms.memsize);

	parms.basedir = basedir; // TODO: "."
// caching is disabled by default, use -cachedir to enable
//	parms.cachedir = cachedir;
	parms.cachedir = nullptr;

	COM_InitArgv(argc, argv);

	parms.argc = com_argc;
	parms.argv = com_argv;
	
	fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) | FNDELAY);

	//printf("Host_Init\n");
    Host_Init(&parms);

	Sys_Init();

	if (COM_CheckParm("-nostdout"))
		nostdout = 1;
	else {
		fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) | FNDELAY);
		printf ("Linux Quake -- Version %0.3f\n", LINUX_VERSION);
	}
	
#ifdef __sun__
	// unroll the simulation loop to give the video side a chance to see _vid_default_mode
	Host_Frame(0.1);
	VID_SetDefaultMode();
#endif

    oldtime = Sys_FloatTime () - 0.1;
    while (1)
    {
// find time spent rendering last frame
        newtime = Sys_FloatTime ();
        time = newtime - oldtime;

        if (cls.state == ca_dedicated)
        {   // play vcrfiles at max speed
            if (time < sys_ticrate.value && (vcrFile == -1 || recording) )
            {
				usleep(1);
                continue;       // not time to run a server only tic yet
            }
            time = sys_ticrate.value;
        }

        if (time > sys_ticrate.value*2)
            oldtime = newtime;
        else
            oldtime += time;

        Host_Frame (time);

// graphic debugging aids
        if (sys_linerefresh.value)
            Sys_LineRefresh ();
    }
	return 0;
}
*/
