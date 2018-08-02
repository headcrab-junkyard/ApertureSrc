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

/*
============================================================================

					LIBRARY REPLACEMENT FUNCTIONS

============================================================================
*/

void Q_memset(void *dest, int fill, int count)
{
	int i;

	if((((long)dest | count) & 3) == 0)
	{
		count >>= 2;
		fill = fill | (fill << 8) | (fill << 16) | (fill << 24);
		for(i = 0; i < count; i++)
			((int *)dest)[i] = fill;
	}
	else
		for(i = 0; i < count; i++)
			((byte *)dest)[i] = fill;
}

void Q_memcpy(void *dest, const void *src, int count)
{
	int i;

	if((((long)dest | (long)src | count) & 3) == 0)
	{
		count >>= 2;
		for(i = 0; i < count; i++)
			((int *)dest)[i] = ((int *)src)[i];
	}
	else
		for(i = 0; i < count; i++)
			((byte *)dest)[i] = ((byte *)src)[i];
}

int Q_memcmp(const void *m1, const void *m2, int count)
{
	while(count)
	{
		count--;
		if(((byte *)m1)[count] != ((byte *)m2)[count])
			return -1;
	}
	return 0;
}

void Q_strcpy(char *dest, const char *src)
{
	while(*src)
	{
		*dest++ = *src++;
	}
	*dest++ = 0;
}

void Q_strncpy(char *dest, const char *src, int count)
{
	while(*src && count--)
	{
		*dest++ = *src++;
	}
	if(count)
		*dest++ = 0;
}

int Q_strlen(const char *str)
{
	int count;

	count = 0;
	while(str[count])
		count++;

	return count;
}

const char *Q_strrchr(const char *s, char c)
{
	int len = Q_strlen(s);
	s += len;

	while(len--)
		if(*--s == c)
			return s;

	return 0;
}

void Q_strcat(char *dest, const char *src)
{
	dest += Q_strlen(dest);
	Q_strcpy(dest, src);
}

int Q_strcmp(const char *s1, const char *s2)
{
	while(1)
	{
		if(*s1 != *s2)
			return -1; // strings not equal
		if(!*s1)
			return 0; // strings are equal
		s1++;
		s2++;
	}

	return -1;
}

int Q_strncmp(const char *s1, const char *s2, int count)
{
	while(1)
	{
		if(!count--)
			return 0;
		if(*s1 != *s2)
			return -1; // strings not equal
		if(!*s1)
			return 0; // strings are equal
		s1++;
		s2++;
	}

	return -1;
}

int Q_strncasecmp(const char *s1, const char *s2, int n)
{
	int c1, c2;

	while(1)
	{
		c1 = *s1++;
		c2 = *s2++;

		if(!n--)
			return 0; // strings are equal until end point

		if(c1 != c2)
		{
			if(c1 >= 'a' && c1 <= 'z')
				c1 -= ('a' - 'A');
			if(c2 >= 'a' && c2 <= 'z')
				c2 -= ('a' - 'A');
			if(c1 != c2)
				return -1; // strings not equal
		}
		if(!c1)
			return 0; // strings are equal
		              //              s1++;
		              //              s2++;
	}

	return -1;
}

int Q_strcasecmp(const char *s1, const char *s2)
{
	return Q_strncasecmp(s1, s2, 99999);
}

int Q_atoi(const char *str)
{
	int val;
	int sign;
	int c;

	if(*str == '-')
	{
		sign = -1;
		str++;
	}
	else
		sign = 1;

	val = 0;

	//
	// check for hex
	//
	if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
	{
		str += 2;
		while(1)
		{
			c = *str++;
			if(c >= '0' && c <= '9')
				val = (val << 4) + c - '0';
			else if(c >= 'a' && c <= 'f')
				val = (val << 4) + c - 'a' + 10;
			else if(c >= 'A' && c <= 'F')
				val = (val << 4) + c - 'A' + 10;
			else
				return val * sign;
		}
	}

	//
	// check for character
	//
	if(str[0] == '\'')
	{
		return sign * str[1];
	}

	//
	// assume decimal
	//
	while(1)
	{
		c = *str++;
		if(c < '0' || c > '9')
			return val * sign;
		val = val * 10 + c - '0';
	}

	return 0;
}

float Q_atof(const char *str)
{
	double val;
	int sign;
	int c;
	int decimal, total;

	if(*str == '-')
	{
		sign = -1;
		str++;
	}
	else
		sign = 1;

	val = 0;

	//
	// check for hex
	//
	if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
	{
		str += 2;
		while(1)
		{
			c = *str++;
			if(c >= '0' && c <= '9')
				val = (val * 16) + c - '0';
			else if(c >= 'a' && c <= 'f')
				val = (val * 16) + c - 'a' + 10;
			else if(c >= 'A' && c <= 'F')
				val = (val * 16) + c - 'A' + 10;
			else
				return val * sign;
		}
	}

	//
	// check for character
	//
	if(str[0] == '\'')
	{
		return sign * str[1];
	}

	//
	// assume decimal
	//
	decimal = -1;
	total = 0;
	while(1)
	{
		c = *str++;
		if(c == '.')
		{
			decimal = total;
			continue;
		}
		if(c < '0' || c > '9')
			break;
		val = val * 10 + c - '0';
		total++;
	}

	if(decimal == -1)
		return val * sign;
	while(total > decimal)
	{
		val /= 10;
		total--;
	}

	return val * sign;
}