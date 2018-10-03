/// @file

#include "BaseItem.hpp"

/*
===============================================================================

ARMOR

===============================================================================
*/

class CItemArmor : public CBaseItem
{
public:
	CItemArmor();
	~CItemArmor();
	
	void Spawn() override;
	void Touch(entvars_t *other) override;
private:
};

CItemArmor::CItemArmor() = default;
CItemArmor::~CItemArmor() = default;

void CItemArmor::Spawn()
{
	self->SetTouchCallback(CItemArmor::Touch);
	
	gpEngine->pfnPrecacheModel("models/armor.mdl");
	gpEngine->pfnSetModel(self, "models/armor.mdl");
	
	self->skin = 0;
	//self->skin = 1; for item_armor2
	//self->skin = 2; for item_armorInv
	
	gpEngine->pfnSetSize(self, '-16 -16 0', '16 16 56');
	
	StartItem(self);
};

void CItemArmor::Touch(entvars_t *other)
{
	float type, value, bit;

	if(other->GetHealth() <= 0)
		return;
	
	if(other->GetClassName() != "player")
		return;

	if(deathmatch == 4)
		if(other->invincible_time > 0)
			return;

	if(self->GetClassName() == "item_armor")
	{
		type = 0.3;
		value = 100;
		bit = IT_ARMOR1;
	};
	
	// TODO: make key-values for these?
	/*
	if(self->GetClassName() == "item_armor2")
	{
		type = 0.6;
		value = 150;
		bit = IT_ARMOR2;
	};
	
	if(self->GetClassName() == "item_armorInv")
	{
		type = 0.8;
		value = 200;
		bit = IT_ARMOR3;
	};
	*/
	
	if(other->armortype * other->armorvalue >= type * value)
		return;

	other->armortype = type;
	other->armorvalue = value;
	other->items = other->items - (other->items & (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3)) + bit;

	self->solid = SOLID_NOT;
	self->model = string_null;
	
	if(deathmatch != 2)
		self->nextthink = time + 20;
	
	self->SetThinkCallback(SUB_regen);

	sprint(other, PRINT_LOW, "You got armor\n");
	
	// armor touch sound
	gpEngine->pfnEmitSound(other, CHAN_ITEM, "items/armor1.wav", 1, ATTN_NORM);
	stuffcmd(other, "bf\n");

	activator = other;
	SUB_UseTargets(); // fire all targets / killtargets
};

/*QUAKED item_armor (0 .5 .8) (-16 -16 0) (16 16 32)
*/
C_EXPORT void item_armor(entvars_t *self)
{
	CItemArmor::Spawn();
};