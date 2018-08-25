/*QUAKED misc_noisemaker (1 0.5 0) (-10 -10 -10) (10 10 10)

For optimzation testing, starts a lot of sounds.
*/
C_EXPORT void misc_noisemaker(entvars_t *self)
{
	CNoiseMaker::Spawn();
};

class CNoiseMaker : public CBaseEntity // CGameEntity
{
public:
	CNoiseMaker();
	~CNoiseMaker();
	
	void Spawn() override;
	void Think() override;
};

void CNoiseMaker::Spawn()
{
	gpEngine->pfnPrecacheSound ("enforcer/enfire.wav");
	gpEngine->pfnPrecacheSound ("enforcer/enfstop.wav");
	gpEngine->pfnPrecacheSound ("enforcer/sight1.wav");
	gpEngine->pfnPrecacheSound ("enforcer/sight2.wav");
	gpEngine->pfnPrecacheSound ("enforcer/sight3.wav");
	gpEngine->pfnPrecacheSound ("enforcer/sight4.wav");
	gpEngine->pfnPrecacheSound ("enforcer/pain1.wav");
	gpEngine->pfnPrecacheSound ("enforcer/pain2.wav");
	gpEngine->pfnPrecacheSound ("enforcer/death1.wav");
	gpEngine->pfnPrecacheSound ("enforcer/idle1.wav");

	self->nextthink = time + 0.1 + random();
	self->SetThinkCallback(CNoiseMaker::Think);
};

void CNoiseMaker::Think()
{
	self->nextthink = time + 0.5;
	
	gpEngine->pfnEmitSound (self, 1, "enforcer/enfire.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 2, "enforcer/enfstop.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 3, "enforcer/sight1.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 4, "enforcer/sight2.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 5, "enforcer/sight3.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 6, "enforcer/sight4.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 7, "enforcer/pain1.wav", 1, ATTN_NORM);
};