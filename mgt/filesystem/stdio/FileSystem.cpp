


//#include <cstdio>
//#include <cstdarg>
#include <cstring>
//#include <cstdlib>
//#include <sys/stat.h>

// TODO: temp
void Sys_Error(const char *msg, ...)
{
};

IFile *CFileSystem::OpenPathID(const char *asFilePath, const char *asPathID)
{
	// TODO
	return nullptr;
};

/*
===========
COM_FindFile

Finds the file in the search path.
Sets com_filesize and one of handle or file
===========
*/
// TODO
/*
IFile *COM_FindFile(const char *filename, int *handle, IFile **file)
{
	searchpath_t *search;
	char netpath[MAX_OSPATH];
	char cachepath[MAX_OSPATH];
	pack_t *pak;
	int i;
	int findtime, cachetime;
	IFile *pFile;

	//
	// search through the path, one element at a time
	//
	search = com_searchpaths;

	for(; search; search = search->next)
	{
		// is the element a pak file?
		if(search->pack)
		{
			// look through all the pak file elements
			pak = search->pack;
			for(i = 0; i < pak->numfiles; i++)
				if(!strcmp(pak->files[i].name, filename))
				{ // found it!
					Sys_Printf("PackFile: %s : %s\n", pak->filename, filename);
					if(handle)
					{
						pak->handle->Seek(pak->files[i].filepos);
					}
					else
					{
						// open a new file on the pakfile
						pFile = FileOpen(pak->filename, "rb");
						if(pFile)
							pFile->Seek(pak->files[i].filepos, SEEK_SET);
					}
					com_filesize = pak->files[i].filelen;
					return pFile;
				}
		}
		else
		{
			// check a file in the directory tree

			sprintf(netpath, "%s/%s", search->filename, filename);

			findtime = GetFileTime(netpath);
			if(findtime == -1)
				continue;

			// see if the file needs to be updated in the cache
			if(!com_cachedir[0])
				strcpy(cachepath, netpath);
			else
			{
#if defined(_WIN32)
				if((strlen(netpath) < 2) || (netpath[1] != ':'))
					sprintf(cachepath, "%s%s", com_cachedir, netpath);
				else
					sprintf(cachepath, "%s%s", com_cachedir, netpath + 2);
#else
				sprintf(cachepath, "%s%s", com_cachedir, netpath);
#endif

				cachetime = GetFileTime(cachepath);

				if(cachetime < findtime)
					COM_CopyFile(netpath, cachepath);
				strcpy(netpath, cachepath);
			}

			Sys_Printf("FindFile: %s\n", netpath);
			pFile = OpenFile(netpath, "rb");
			com_filesize = pFile->GetSize();
			return pFile;
		}
	}

	Sys_Printf("FindFile: can't find %s\n", filename);
	com_filesize = -1;
	return nullptr;
}
*/

/*
============
COM_CloseFile

If it is a pak file handle, don't really close it
============
*/
void CFileSystem::CloseFile(IFile *apFile)
{
	// If it is a pak file handle, don't really close it
	for(searchpath_t *s = com_searchpaths; s; s = s->next)
		if(s->pack && s->pack->handle == apFile)
			return;
	
	if(!apFile)
		return;
	
	// TODO: free the file
	
	//auto It{mlstOpenHandles.find(apFile)};
	
	//if(It)
	{
		//delete It;
		//mlstOpenHandles.erase(It);
	};
};

/*
int CFileSystem::FileOpen(const char *path, const char *mode)
{
	// FS_FileOpenRead
	//int Sys_FileOpenRead(char *path, int *hndl)
	/*
	FILE *f;
	int i, retval;
	
	//int t = VID_ForceUnlockedAndReturnState(); // TODO
	
	i = findhandle ();

	f = fopen(path, "rb");
	if (!f)
	{
		*hndl = -1;
		return -1; // retval = -1 // TODO
	}
	else
	{
		sys_handles[i] = f;
		*hndl = i;
		retval = filelength(f);
	};
	
	//VID_ForceLockState(t); // TODO
	
	return retval;
	
	*/
	//
	
	// TODO
	/*
	//int t{VID_ForceUnlockedAndReturnState()}; // TODO: non-dedicated win only
	
	FILE *f{fopen(path, "wb")};
	
	if(!f)
		Sys_Error("Error opening %s: %s", path, strerror(errno));
	
	int i{findhandle()};
	
	sys_handles[i] = f;
	
	//VID_ForceLockState(t); // TODO: non-dedicated win only
	
	return i;
};
*/

/*
void CFileSystem::FileClose(int handle)
{
	if(!handle)
		return;

	//int t = VID_ForceUnlockedAndReturnState(); // TODO: windows & not dedicated
	
#ifdef _WIN32
	fclose(sys_handles[handle]);
	sys_handles[handle] = nullptr;
#else
	close(handle);
#endif

	//VID_ForceLockState(t); // TODO: windows & not dedicated
};
*/

void CFileSystem::FileSeek(int handle, int position) // TODO: seek mode
{
	//int t = VID_ForceUnlockedAndReturnState(); // TODO: windows & not dedicated
	
#ifdef _WIN32
	fseek(sys_handles[handle], position, SEEK_SET);
#else
	lseek(handle, position, SEEK_SET);
#endif

	//VID_ForceLockState(t); // TODO: windows & not dedicated
};

int CFileSystem::FileRead(int handle, void *dest, int count)
{
	if(!dest)
		return -1;
	
	//int t = VID_ForceUnlockedAndReturnState(); // TODO
	
	int x;
	
#ifdef _WIN32
	x = fread(dest, 1, count, sys_handles[handle]);
#else
	x = read(handle, dest, count);
#endif

	//VID_ForceLockState(t); // TODO
	return x;
};

int CFileSystem::FileWrite(int handle, const void *data, int count)
{
	if(!data)
		return -1;
	
	//int t = VID_ForceUnlockedAndReturnState(); // TODO
	
	int x;
	
#ifdef _WIN32
	x = fwrite(data, 1, count, sys_handles[handle]);
#else
	x = write(handle, src, count);
#endif

	//VID_ForceLockState(t); // TODO
	return x;
};

int CFileSystem::findhandle()
{
	for(int i = 1; i < MAX_HANDLES; i++)
		if(!sys_handles[i]) // TODO: if(!sys_handles[i].hFile) for sun
			return i;
	
	Sys_Error("out of handles");
	return -1;
};

/*
================
filelength
================
*/
int CFileSystem::filelength(FILE *f)
{
	//int t = VID_ForceUnlockedAndReturnState(); // TODO: windows & not dedicated
	
	int pos = ftell(f);
	fseek(f, 0, SEEK_END);
	
	int end = ftell(f);
	fseek(f, pos, SEEK_SET);

	//VID_ForceLockState(t); // TODO: windows & not dedicated
	
	return end;
};