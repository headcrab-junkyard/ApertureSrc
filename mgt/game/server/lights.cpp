const float START_OFF = 1;

void CLight::Use()
{
	if (self->spawnflags & START_OFF)
	{
		gpEngine->pfnLightStyle(self->style, "m");
		self->spawnflags = self->spawnflags - START_OFF;
	}
	else
	{
		gpEngine->pfnLightStyle(self->style, "a");
		self->spawnflags = self->spawnflags + START_OFF;
	};
};

void CLight::OnSpawn()
{
	if(!self->targetname)
	{
		// inert light
		remove(self);
		return;
	};
	
	if (self->style >= 32)
	{
		self->SetUseCallback(CLight::Use);
		
		if (self->spawnflags & START_OFF)
			gpEngine->pfnLightStyle(self->style, "a");
		else
			gpEngine->pfnLightStyle(self->style, "m");
	};
};

/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) START_OFF
Non-displayed light.
Default light value is 300
Default style is 0
If targeted, it will toggle between on or off.
*/
C_EXPORT void light(entvars_t *self)
{
	CLight::OnSpawn();
};

/*QUAKED light_fluoro (0 1 0) (-8 -8 -8) (8 8 8) START_OFF
Non-displayed light.
Default light value is 300
Default style is 0
If targeted, it will toggle between on or off.
Makes steady fluorescent humming sound
*/
C_EXPORT void light_fluoro(entvars_t *self)
{
	if (self->style >= 32)
	{
		self->SetUseCallback(CLight::Use);
		
		if (self->spawnflags & START_OFF)
			gpEngine->pfnLightStyle(self->style, "a");
		else
			gpEngine->pfnLightStyle(self->style, "m");
	};
	
	gpEngine->pfnPrecacheSound ("ambience/fl_hum1.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/fl_hum1.wav", 0.5, ATTN_STATIC);
};

/*QUAKED light_fluorospark (0 1 0) (-8 -8 -8) (8 8 8)
Non-displayed light.
Default light value is 300
Default style is 10
Makes sparking, broken fluorescent sound
*/
C_EXPORT void light_fluorospark(entvars_t *self)
{
	if (!self->style)
		self->style = 10;

	gpEngine->pfnPrecacheSound ("ambience/buzz1.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/buzz1.wav", 0.5, ATTN_STATIC);
};

/*QUAKED light_globe (0 1 0) (-8 -8 -8) (8 8 8)
Sphere globe light.
Default light value is 300
Default style is 0
*/
C_EXPORT void light_globe(entvars_t *self)
{
	gpEngine->pfnPrecacheModel ("sprites/s_light.spr");
	gpEngine->pfnSetModel (self, "sprites/s_light.spr");
	gpEngine->pfnMakeStatic (self);
};

void FireAmbient(entvars_t *self)
{
	gpEngine->pfnPrecacheSound ("ambience/fire1.wav");
// attenuate fast
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/fire1.wav", 0.5, ATTN_STATIC);
};

/*QUAKED light_torch_small_walltorch (0 .5 0) (-10 -10 -20) (10 10 20)
Short wall torch
Default light value is 200
Default style is 0
*/
C_EXPORT void light_torch_small_walltorch(entvars_t *self)
{
	gpEngine->pfnPrecacheModel ("models/flame.mdl");
	gpEngine->pfnSetModel (self, "models/flame.mdl");
	FireAmbient (self);
	gpEngine->pfnMakeStatic (self);
};

/*QUAKED light_flame_large_yellow (0 1 0) (-10 -10 -12) (12 12 18)
Large yellow flame ball
*/
C_EXPORT void light_flame_large_yellow(entvars_t *self)
{
	gpEngine->pfnPrecacheModel ("models/flame2.mdl");
	gpEngine->pfnSetModel (self, "models/flame2.mdl");
	self->frame = 1;
	FireAmbient (self);
	gpEngine->pfnMakeStatic (self);
};

/*QUAKED light_flame_small_yellow (0 1 0) (-8 -8 -8) (8 8 8) START_OFF
Small yellow flame ball
*/
C_EXPORT void light_flame_small_yellow(entvars_t *self)
{
	gpEngine->pfnPrecacheModel ("models/flame2.mdl");
	gpEngine->pfnSetModel (self, "models/flame2.mdl");
	FireAmbient (self);
	gpEngine->pfnMakeStatic (self);
};

/*QUAKED light_flame_small_white (0 1 0) (-10 -10 -40) (10 10 40) START_OFF
Small white flame ball
*/
C_EXPORT void light_flame_small_white(entvars_t *self)
{
	gpEngine->pfnPrecacheModel ("models/flame2.mdl");
	gpEngine->pfnSetModel (self, "models/flame2.mdl");
	FireAmbient (self);
	gpEngine->pfnMakeStatic (self);
};