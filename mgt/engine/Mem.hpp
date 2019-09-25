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
/// @brief dynamic memory management functions

#pragma once

/// Allocate memory block
/// @param size - amount of memory (in bytes) to allocate
/// @return pointer to allocated memory block
void *Mem_Alloc(size_t size);

/// Allocate (number * size) bytes and zero it
/// @param number - TODO
/// @param size - TODO
/// @return pointer to allocated memory block
void *Mem_Calloc(int number, size_t size);

/// Reallocate memory block
/// @param ptr - pointer to a memory block to reallocate
/// @param size - new size of the memory block
/// @return pointer to allocated memory block
void *Mem_Realloc(void *ptr, size_t size);

/// Deallocate memory block
/// @param ptr - pointer to a memory block to free
void Mem_Free(void *ptr);