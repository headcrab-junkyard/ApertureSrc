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
#include "tier1/interface.h"

struct IFileSystem;
class KeyValues;

#ifndef _WCHAR_T_DEFINED
using wchar_t = unsigned short;
#define _WCHAR_T_DEFINED
#endif

constexpr auto VGUI_LOCALIZE_INTERFACE_VERSION{"VGUI_Localize003"};

namespace vgui3
{

using StringIndex_t = unsigned long;
const unsigned long INVALID_STRING_INDEX = (unsigned long)-1;

struct ILocalize : public IBaseInterface
{
	virtual bool AddFile(IFileSystem *fileSystem, const char *fileName) = 0;
	virtual void RemoveAll() = 0;
	virtual wchar_t *Find(char const *tokenName) = 0;
	virtual int ConvertANSIToUnicode(const char *ansi, wchar_t *unicode, int unicodeBufferSizeInBytes) = 0;
	virtual int ConvertUnicodeToANSI(const wchar_t *unicode, char *ansi, int ansiBufferSize) = 0;
	virtual StringIndex_t FindIndex(const char *tokenName) = 0;
	virtual void ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, wchar_t *formatString, int numFormatParameters, ...) = 0;
	virtual const char *GetNameByIndex(StringIndex_t index) = 0;
	virtual wchar_t *GetValueByIndex(StringIndex_t index) = 0;
	virtual StringIndex_t GetFirstStringIndex() = 0;
	virtual StringIndex_t GetNextStringIndex(StringIndex_t index) = 0;
	virtual void AddString(const char *tokenName, wchar_t *unicodeString, const char *fileName) = 0;
	virtual void SetValueByIndex(StringIndex_t index, wchar_t *newValue) = 0;
	virtual bool SaveToFile(IFileSystem *fileSystem, const char *fileName) = 0;
	virtual int GetLocalizationFileCount() = 0;
	virtual const char *GetLocalizationFileName(int index) = 0;
	virtual const char *GetFileNameByIndex(StringIndex_t index) = 0;
	virtual void ReloadLocalizationFiles() = 0;
	virtual void ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, const char *tokenName, KeyValues *localizationVariables) = 0;
	virtual void ConstructStringByIndex(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, StringIndex_t unlocalizedTextSymbol, KeyValues *localizationVariables) = 0;
};

}; // namespace vgui3