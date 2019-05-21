/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018-2019 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

#include "VGUI.hpp"
#include "KeyCode.hpp"
#include "tier1/interface.h"

#ifdef PlaySound
#undef PlaySound
#endif

constexpr auto VGUI_SYSTEM_INTERFACE_VERSION{"VGUI_System009"};

class KeyValues;

namespace vgui3
{

struct ISystem : public IBaseInterface
{
	///
	virtual void Shutdown() = 0;
	
	///
	virtual void RunFrame() = 0;
	
	///
	virtual void ShellExecute(const char *command, const char *file) = 0;
	
	///
	virtual double GetFrameTime() const = 0;
	
	///
	virtual double GetCurrentTime() const = 0;
	
	///
	virtual long GetTimeMillis() const = 0;
	
	///
	virtual int GetClipboardTextCount() = 0;
	
	///
	virtual void SetClipboardText(const char *text, int textLen) = 0;
	
	///
	virtual void SetClipboardText(const wchar_t *text, int textLen) = 0;
	
	///
	virtual int GetClipboardText(int offset, char *buf, int bufLen) = 0;
	
	///
	virtual int GetClipboardText(int offset, wchar_t *buf, int bufLen) = 0;
	
	///
	virtual bool SetRegistryString(const char *key, const char *value) = 0;
	
	///
	virtual bool GetRegistryString(const char *key, char *value, int valueLen) const = 0;
	
	///
	virtual bool SetRegistryInteger(const char *key, int value) = 0;
	
	///
	virtual bool GetRegistryInteger(const char *key, int &value) const = 0;
	
	///
	virtual KeyValues *GetUserConfigFileData(const char *dialogName, int dialogID) = 0;
	
	///
	virtual void SetUserConfigFile(const char *fileName, const char *pathName) = 0;
	
	///
	virtual void SaveUserConfigFile() = 0;
	
	///
	virtual bool SetWatchForComputerUse(bool state) = 0;
	
	///
	virtual double GetTimeSinceLastUse() const = 0;
	
	///
	virtual int GetAvailableDrives(char *buf, int bufLen) = 0;
	
	///
	virtual bool CommandLineParamExists(const char *paramName) const = 0;
	
	///
	virtual const char *GetFullCommandLine() const = 0;
	
	///
	virtual bool GetCurrentTimeAndDate(int *year, int *month, int *dayOfWeek, int *day, int *hour, int *minute, int *second) = 0;
	
	///
	virtual double GetFreeDiskSpace(const char *path) const = 0;
	
	///
	virtual bool CreateShortcut(const char *linkFileName, const char *targetPath, const char *arguments, const char *workingDirectory, const char *iconFile) = 0;
	
	///
	virtual bool GetShortcutTarget(const char *linkFileName, char *targetPath, char *arguments, int destBufferSizes) = 0;
	
	///
	virtual bool ModifyShortcutTarget(const char *linkFileName, const char *targetPath, const char *arguments, const char *workingDirectory) = 0;
	
	///
	virtual bool GetCommandLineParamValue(const char *paramName, char *value, int valueBufferSize) = 0;
	
	///
	virtual bool DeleteRegistryKey(const char *keyName) = 0;
	
	///
	virtual const char *GetDesktopFolderPath() const = 0;
};

}; // namespace vgui3