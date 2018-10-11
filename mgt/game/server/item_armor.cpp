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
	void Touch(CBaseEntity *other) override;
private:
};

CItemArmor::CItemArmor() = default;
CItemArmor::~CItemArmor() = default;

void CItemArmor::Spawn()
{
	SetTouchCallback(CItemArmor::Touch);
	
	gpEngine->pfnPrecacheModel("models/armor.mdl");
	SetModel("models/armor.mdl");
	
	self->skin = 0;
	//self->skin = 1; for item_armor2
	//self->skin = 2; for item_armorInv
	
	SetSize(vec3_t{-16.0f, -16.0f, 0.0f}, vec3_t{16.0f, 16.0f, 56.0f});
	
	StartItem(self);
};

void CItemArmor::Touch(CBaseEntity *other)
{
	if(other->GetHealth() <= 0)
		return;
	
	if(other->GetClassName() != "player")
		return;

	if(deathmatch == 4)
		if(other->invincible_time > 0)
			return;

	float type{0.0f}, value{0.0f}, bit{0.0f};
	
	if(self->GetClassName() == "item_armor")
	{
		type = 0.3f;
		value = 100.0f;
		bit = IT_ARMOR1;
	};
	
	// TODO: make key-values for these?
	/*
	if(self->GetClassName() == "item_armor2")
	{
		type = 0.6f;
		value = 150.0f;
		bit = IT_ARMOR2;
	};
	
	if(self->GetClassName() == "item_armorInv")
	{
		type = 0.8f;
		value = 200.0f;
		bit = IT_ARMOR3;
	};
	*/
	
	if(other->GetArmorType() * other->GetArmorValue() >= type * value)
		return;

	other->SetArmorType(type);
	other->SetArmorValue(value);
	other->items = other->items - (other->items & (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3)) + bit;

	self->solid = SOLID_NOT;
	self->SetModel(string_null);
	
	if(deathmatch != 2)
		self->SetNextThink(time + 20);
	
	self->SetThinkCallback(SUB_regen);

	sprint(other, PRINT_LOW, "You got armor\n");
	
	// armor touch sound
	other->EmitSound(CHAN_ITEM, "items/armor1.wav", 1, ATTN_NORM);
	
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