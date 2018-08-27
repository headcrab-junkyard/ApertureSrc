

mod_init

/*
===============
Mod_Extradata

Caches the data if needed
===============
*/
// TODO: unused on server-side
void *Mod_Extradata (model_t *mod)
{
	void	*r;
	
	r = Cache_Check (&mod->cache);
	if (r)
		return r;

	Mod_LoadModel (mod, true);
	
	if (!mod->cache.data)
		Sys_Error ("Mod_Extradata: caching failed");
	return mod->cache.data;
}

mod_pointinleaf

mod_decompressvis

mod_leafpvs

mod_clearall

mod_findname

/*
==================
Mod_TouchModel

==================
*/
// TODO: unused on server-side
void Mod_TouchModel(const char *name)
{
	model_t	*mod{Mod_FindName(name)};
	
	if (mod->needload == NL_PRESENT)
	{
		if (mod->type == mod_alias)
			Cache_Check (&mod->cache);
	}
}

mod_loadmodel

mod_forname