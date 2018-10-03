/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#pragma once

class CInventory
{
public:
	CInventory() { Clear(); }
	~CInventory() { Clear(); }
	
	// save games
	void Save(idSaveGame *savefile) const; // archives object for save game file
	void Restore(idRestoreGame *savefile); // unarchives object from save game file

	void Clear();
	void GivePowerUp(CBasePlayer *player, int powerup, int msec);
	void ClearPowerUps();
	void GetPersistantData(CDict &dict);
	void RestoreInventory(CBasePlayer *owner, const idDict &dict);
	bool Give(CBasePlayer *owner, const CDict &spawnArgs, const char *statname, const char *value, CPredictedValue<int> *idealWeapon, bool updateHud, unsigned int giveFlags);
	void Drop(const CDict &spawnArgs, const char *weapon_classname, int weapon_index);
	ammo_t AmmoIndexForAmmoClass(const char *ammo_classname) const;
	int MaxAmmoForAmmoClass(const CBasePlayer *owner, const char *ammo_classname) const;
	int WeaponIndexForAmmoClass(const CDict &spawnArgs, const char *ammo_classname) const;
	ammo_t AmmoIndexForWeaponClass(const char *weapon_classname, int *ammoRequired);
	const char *AmmoPickupNameForIndex(ammo_t ammonum) const;
	void AddPickupName(const char *name, CBasePlayer *owner); //_D3XP

	int HasAmmo(ammo_t type, int amount);
	bool UseAmmo(ammo_t type, int amount);
	int HasAmmo(const char *weapon_classname, bool includeClip = false, CBasePlayer *owner = nullptr); // _D3XP

	bool HasEmptyClipCannotRefill(const char *weapon_classname, CBasePlayer *owner);

	void UpdateArmor();

	void SetInventoryAmmoForType(const int ammoType, const int amount);
	void SetClipAmmoForWeapon(const int weapon, const int amount);

	int GetInventoryAmmoForType(const int ammoType) const;
	int GetClipAmmoForWeapon(const int weapon) const;

	void WriteAmmoToSnapshot(CBitMsg &msg) const;
	void ReadAmmoFromSnapshot(const CBitMsg &msg, int ownerEntityNumber);

	void SetRemoteClientAmmo(const int ownerEntityNumber);

	int nextItemPickup;
	int nextItemNum;
	int onePickupTime;
	CList<CStr> pickupItemNames;
	CList<CObjectiveInfo> objectiveNames;

	void InitRechargeAmmo(CBasePlayer *owner);
	void RechargeAmmo(CBasePlayer *owner);
	bool CanGive(CBasePlayer *owner, const CDict &spawnArgs, const char *statname, const char *value);
	
	int maxHealth;
	int weapons;
	int powerups;
	int armor;
	int maxarmor;
	int powerupEndTime[MAX_POWERUPS];

	RechargeAmmo_t rechargeAmmo[AMMO_NUMTYPES];

	// mp
	int ammoPredictTime; // Unused now but kept for save file compatibility.

	int deplete_armor;
	float deplete_rate;
	int deplete_ammount;
	int nextArmorDepleteTime;

	int pdasViewed[4]; // 128 bit flags for indicating if a pda has been viewed

	int selPDA;
	int selEMail;
	int selVideo;
	int selAudio;
	
	bool pdaOpened;
	
	CList<CDict *> items;
	CList<CStr> pdaSecurity;
	CList<const idDeclPDA *> pdas;
	CList<const idDeclVideo *> videos;
	CList<const idDeclEmail *> emails;

	bool ammoPulse;
	bool weaponPulse;
	bool armorPulse;
	int lastGiveTime;

	CList<CLevelTriggerInfo, TAG_IDLIB_LIST_PLAYER> levelTriggers;
private:
	CArray<CPredictedValue<int>, AMMO_NUMTYPES> ammo;
	CArray<CPredictedValue<int>, MAX_WEAPONS> clip;
};