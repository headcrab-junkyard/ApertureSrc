/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2022 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief engine module interface

#pragma once

#include "CommonTypes.hpp"
#include "tier1/interface.h"

constexpr auto OGS_ENGINEMODULE_INTERFACE_VERSION{"OGSEngineModule001"};

// TODO: IEngineSystem?
interface IEngineModule : public IBaseInterface
{
    ///
    virtual bool Init(CreateInterfaceFn afnEngineFactory) = 0;

    ///
    virtual void Shutdown() = 0;

    ///
    virtual IBaseInterface *QueryInterface(const char *asName) const = 0;

    ///
    template<typename T>
    inline T *QueryInterface(const char *asName) const {return reinterpret_cast<T*>(QueryInterface(asName));}
};

class CBaseEngineModule : public IEngineModule
{
public:
    IBaseInterface *QueryInterface(const char *asName) const override
    {
        auto pLocalModuleFactory{Sys_GetFactoryThis()};
        return pLocalModuleFactory(asName);
    };
};