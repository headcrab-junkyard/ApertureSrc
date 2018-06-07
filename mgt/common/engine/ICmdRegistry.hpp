/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
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
/// @brief command registry interface

#pragma once

#include "CommonTypes.hpp"

using pfnCmdCallback = void (*)();

constexpr auto MGT_CLIENTCMDREGISTRY_INTERFACE_VERSION{"MGTClientCmdRegistry001Alpha"};
constexpr auto MGT_SERVERCMDREGISTRY_INTERFACE_VERSION{"MGTServerCmdRegistry001Alpha"};

interface ICmdRegistry
{
	///
	virtual void AddCommand(const char *asName, pfnCmdCallback afnCallback) = 0;
};