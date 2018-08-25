/*
=========================================================================

HEALTH BOX

=========================================================================
*/

//
// T_Heal: add health to an entity, limiting health to max_health
// "ignore" will ignore max_health limit
//
bool T_Heal(CBaseEntity *e, float healamount, float ignore)
{
	if (e->health <= 0)
		return false;
	
	if ((!ignore) && (e->health >= other.max_health))
		return false;
	
	healamount = ceil(healamount);

	e->health += healamount;
	
	if ((!ignore) && (e->health >= other.max_health))
		e->health = other.max_health;
		
	if (e->health > 250)
		e->health = 250;
	
	return true;
};

/*QUAKED item_health (.3 .3 1) (0 0 0) (32 32 32) rotten megahealth
Health box. Normally gives 25 points.
Rotten box heals 5-10 points,
megahealth will add 100 health, then 
rot you down to your maximum health limit, 
one point per second.
*/

const float H_ROTTEN = 1;
const float H_MEGA = 2;

void item_megahealth_rot();

C_EXPORT void item_health(entvars_t *self)
{
	CItemHealth::OnSpawn();
};

class CItemHealth : public CBaseEntity
{
public:
	CItemHealth();
	~CItemHealth();
	
	void OnSpawn();
	
	void OnTouch(CBaseEntity *other);
private:
	float healamount, healtype;
};

void CItemHealth::OnSpawn()
{
	self.SetTouchFunc(CItemHealth::OnTouch); // SetTouchCallback

	if (self.spawnflags & H_ROTTEN)
	{
		gpEngine->pfnPrecacheModel("maps/b_bh10.bsp");

		gpEngine->pfnPrecacheSound("items/r_item1.wav");
		gpEngine->pfnSetModel(self, "maps/b_bh10.bsp");
		
		self.noise = "items/r_item1.wav";
		
		this->healamount = 15;
		this->healtype = 0;
	}
	else
		if (self.spawnflags & H_MEGA)
		{
			gpEngine->pfnPrecacheModel("maps/b_bh100.bsp");
			gpEngine->pfnPrecacheSound("items/r_item2.wav");
			gpEngine->pfnSetModel(self, "maps/b_bh100.bsp");
			
			self.noise = "items/r_item2.wav";
			
			this->healamount = 100;
			this->healtype = 2;
		}
		else
		{
			gpEngine->pfnPrecacheModel("maps/b_bh25.bsp");
			gpEngine->pfnPrecacheSound("items/health1.wav");
			gpEngine->pfnSetModel(self, "maps/b_bh25.bsp");
			
			self.noise = "items/health1.wav";
			
			this->healamount = 25;
			this->healtype = 1;
		};
	
	gpEngine->pfnSetSize (self, '0 0 0', '32 32 56');
	StartItem ();
};

void CItemHealth::OnTouch(CBaseEntity *other)
{
	float amount;
	string  s;
	
	if (deathmatch == 4)
		if (other.invincible_time > 0)
			return;

	if (other.classname != "player")
		return;
	
	if (this->healtype == 2) // Megahealth?  Ignore max_health...
	{
		if (other.health >= 250)
			return;
		if (!T_Heal(other, this->healamount, 1))
			return;
	}
	else
		if (!T_Heal(other, this->healamount, 0))
			return;
	
	sprint(other, PRINT_LOW, "You receive ");
	s = ftos(this->healamount);
	sprint(other, PRINT_LOW, s);
	sprint(other, PRINT_LOW, " health\n");
	
	// health touch sound
	gpEngine->pfnEmitSound(other, CHAN_ITEM, self.noise, 1, ATTN_NORM);

	stuffcmd (other, "bf\n");
	
	self.model = string_null;
	self.solid = SOLID_NOT;

	// Megahealth = rot down the player's super health
	if (self.healtype == 2)
	{
		other.items = other.items | IT_SUPERHEALTH;
		if (deathmatch != 4)
		{
			self.nextthink = time + 5;
			self.think = item_megahealth_rot;
		};
		self.owner = other;
	}
	else
	{
		if (deathmatch != 2)            // deathmatch 2 is the silly old rules
		{
			self.nextthink = time + 20;
			self.think = SUB_regen;
		};
	};
	
	activator = other;
	SUB_UseTargets();                               // fire all targets / killtargets
};      

void item_megahealth_rot()
{
	other = self.owner;
	
	if (other.health > other.max_health)
	{
		other.health = other.health - 1;
		self.nextthink = time + 1;
		return;
	};

// it is possible for a player to die and respawn between rots, so don't
// just blindly subtract the flag off
	other.items = other.items - (other.items & IT_SUPERHEALTH);
	
	if (deathmatch != 2)    // deathmatch 2 is silly old rules
	{
		self.nextthink = time + 20;
		self.think = SUB_regen;
	};
};