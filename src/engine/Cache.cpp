#include "quakedef.h"
#include "Cache.hpp"

void *CCache::Alloc(cache_user_t *c, int size, const char *name)
{
	return Cache_Alloc(c, size, name);
};

void CCache::Free(cache_user_t *c)
{
	Cache_Free(c);
};

void *CCache::Check(cache_user_t *c)
{
	return Cache_Check(c);
};

void CCache::Report()
{
	Cache_Report();
};

void CCache::Flush()
{
	Cache_Flush();
};