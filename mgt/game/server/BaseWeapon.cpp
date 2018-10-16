/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
 * Copyright (C) 2018 BlackPhrase
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
 *
 * In addition, Magenta Engine is also subject to certain additional terms. 
 * You should have received a copy of these additional terms immediately 
 * following the terms and conditions of the GNU General Public License 
 * which accompanied the Magenta Engine. If not, please request a copy 
 * in writing from id Software at the address below.
 *
 * If you have questions concerning this license or the applicable 
 * additional terms, you may contact in writing id Software LLC, 
 * c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.
*/

/// @file

#include "BaseWeapon.hpp"

CBaseWeapon::CBaseWeapon() = default;
CBaseWeapon::~CBaseWeapon() = default;

void CBaseWeapon::Spawn(){};

/*
================
CBaseWeapon::Save
================
*/
void CBaseWeapon::Save(idSaveGame *savefile) const
{
	savefile->WriteInt(status);
	savefile->WriteObject(thread);
	savefile->WriteString(state);
	savefile->WriteString(idealState);
	savefile->WriteInt(animBlendFrames);
	savefile->WriteInt(animDoneTime);
	savefile->WriteBool(isLinked);

	savefile->WriteObject(owner);
	worldModel.Save(savefile);

	savefile->WriteInt(hideTime);
	savefile->WriteFloat(hideDistance);
	savefile->WriteInt(hideStartTime);
	savefile->WriteFloat(hideStart);
	savefile->WriteFloat(hideEnd);
	savefile->WriteFloat(hideOffset);
	savefile->WriteBool(hide);
	savefile->WriteBool(disabled);

	savefile->WriteInt(berserk);

	savefile->WriteVec3(playerViewOrigin);
	savefile->WriteMat3(playerViewAxis);

	savefile->WriteVec3(viewWeaponOrigin);
	savefile->WriteMat3(viewWeaponAxis);

	savefile->WriteVec3(muzzleOrigin);
	savefile->WriteMat3(muzzleAxis);

	savefile->WriteVec3(pushVelocity);

	savefile->WriteString(weaponDef->GetName());
	savefile->WriteFloat(meleeDistance);
	savefile->WriteString(meleeDefName);
	savefile->WriteInt(brassDelay);
	savefile->WriteString(icon);
	savefile->WriteString(pdaIcon);
	savefile->WriteString(displayName);
	savefile->WriteString(itemDesc);

	savefile->WriteInt(guiLightHandle);
	savefile->WriteRenderLight(guiLight);

	savefile->WriteInt(muzzleFlashHandle);
	savefile->WriteRenderLight(muzzleFlash);

	savefile->WriteInt(worldMuzzleFlashHandle);
	savefile->WriteRenderLight(worldMuzzleFlash);

	savefile->WriteVec3(flashColor);
	savefile->WriteInt(muzzleFlashEnd);
	savefile->WriteInt(flashTime);

	savefile->WriteBool(lightOn);
	savefile->WriteBool(silent_fire);

	savefile->WriteInt(kick_endtime);
	savefile->WriteInt(muzzle_kick_time);
	savefile->WriteInt(muzzle_kick_maxtime);
	savefile->WriteAngles(muzzle_kick_angles);
	savefile->WriteVec3(muzzle_kick_offset);

	savefile->WriteInt(ammoType);
	savefile->WriteInt(ammoRequired);
	savefile->WriteInt(clipSize);
	savefile->WriteInt(ammoClip.Get());
	savefile->WriteInt(lowAmmo);
	savefile->WriteBool(powerAmmo);

	// savegames <= 17
	savefile->WriteInt(0);

	savefile->WriteInt(zoomFov);

	savefile->WriteJoint(barrelJointView);
	savefile->WriteJoint(flashJointView);
	savefile->WriteJoint(ejectJointView);
	savefile->WriteJoint(guiLightJointView);
	savefile->WriteJoint(ventLightJointView);

	savefile->WriteJoint(flashJointWorld);
	savefile->WriteJoint(barrelJointWorld);
	savefile->WriteJoint(ejectJointWorld);

	savefile->WriteBool(hasBloodSplat);

	savefile->WriteSoundShader(sndHum);

	savefile->WriteParticle(weaponSmoke);
	savefile->WriteInt(weaponSmokeStartTime);
	savefile->WriteBool(continuousSmoke);
	savefile->WriteParticle(strikeSmoke);
	savefile->WriteInt(strikeSmokeStartTime);
	savefile->WriteVec3(strikePos);
	savefile->WriteMat3(strikeAxis);
	savefile->WriteInt(nextStrikeFx);

	savefile->WriteBool(nozzleFx);
	savefile->WriteInt(nozzleFxFade);

	savefile->WriteInt(lastAttack);

	savefile->WriteInt(nozzleGlowHandle);
	savefile->WriteRenderLight(nozzleGlow);

	savefile->WriteVec3(nozzleGlowColor);
	savefile->WriteMaterial(nozzleGlowShader);
	savefile->WriteFloat(nozzleGlowRadius);

	savefile->WriteInt(weaponAngleOffsetAverages);
	savefile->WriteFloat(weaponAngleOffsetScale);
	savefile->WriteFloat(weaponAngleOffsetMax);
	savefile->WriteFloat(weaponOffsetTime);
	savefile->WriteFloat(weaponOffsetScale);

	savefile->WriteBool(allowDrop);
	savefile->WriteObject(projectileEnt);

	savefile->WriteStaticObject(grabber);
	savefile->WriteInt(grabberState);

	savefile->WriteJoint(smokeJointView);

	savefile->WriteInt(weaponParticles.Num());
	for(int i = 0; i < weaponParticles.Num(); i++)
	{
		WeaponParticle_t *part = weaponParticles.GetIndex(i);
		savefile->WriteString(part->name);
		savefile->WriteString(part->particlename);
		savefile->WriteBool(part->active);
		savefile->WriteInt(part->startTime);
		savefile->WriteJoint(part->joint);
		savefile->WriteBool(part->smoke);
		if(!part->smoke)
			savefile->WriteObject(part->emitter);
	};
	savefile->WriteInt(weaponLights.Num());
	for(int i = 0; i < weaponLights.Num(); i++)
	{
		WeaponLight_t *light = weaponLights.GetIndex(i);
		savefile->WriteString(light->name);
		savefile->WriteBool(light->active);
		savefile->WriteInt(light->startTime);
		savefile->WriteJoint(light->joint);
		savefile->WriteInt(light->lightHandle);
		savefile->WriteRenderLight(light->light);
	};
};

/*
================
CBaseWeapon::Restore
================
*/
void CBaseWeapon::Restore(idRestoreGame *savefile)
{
	savefile->ReadInt((int &)status);
	savefile->ReadObject(reinterpret_cast<idClass *&>(thread));
	savefile->ReadString(state);
	savefile->ReadString(idealState);
	savefile->ReadInt(animBlendFrames);
	savefile->ReadInt(animDoneTime);
	savefile->ReadBool(isLinked);

	// Re-link script fields
	WEAPON_ATTACK.LinkTo(scriptObject, "WEAPON_ATTACK");
	WEAPON_RELOAD.LinkTo(scriptObject, "WEAPON_RELOAD");
	WEAPON_NETRELOAD.LinkTo(scriptObject, "WEAPON_NETRELOAD");
	WEAPON_NETENDRELOAD.LinkTo(scriptObject, "WEAPON_NETENDRELOAD");
	WEAPON_NETFIRING.LinkTo(scriptObject, "WEAPON_NETFIRING");
	WEAPON_RAISEWEAPON.LinkTo(scriptObject, "WEAPON_RAISEWEAPON");
	WEAPON_LOWERWEAPON.LinkTo(scriptObject, "WEAPON_LOWERWEAPON");

	savefile->ReadObject(reinterpret_cast<idClass *&>(owner));
	worldModel.Restore(savefile);

	savefile->ReadInt(hideTime);
	savefile->ReadFloat(hideDistance);
	savefile->ReadInt(hideStartTime);
	savefile->ReadFloat(hideStart);
	savefile->ReadFloat(hideEnd);
	savefile->ReadFloat(hideOffset);
	savefile->ReadBool(hide);
	savefile->ReadBool(disabled);

	savefile->ReadInt(berserk);

	savefile->ReadVec3(playerViewOrigin);
	savefile->ReadMat3(playerViewAxis);

	savefile->ReadVec3(viewWeaponOrigin);
	savefile->ReadMat3(viewWeaponAxis);

	savefile->ReadVec3(muzzleOrigin);
	savefile->ReadMat3(muzzleAxis);

	savefile->ReadVec3(pushVelocity);

	idStr objectname;
	savefile->ReadString(objectname);
	weaponDef = gameLocal.FindEntityDef(objectname);
	meleeDef = gameLocal.FindEntityDef(weaponDef->dict.GetString("def_melee"), false);

	const idDeclEntityDef *projectileDef = gameLocal.FindEntityDef(weaponDef->dict.GetString("def_projectile"), false);
	if(projectileDef)
		projectileDict = projectileDef->dict;
	else
		projectileDict.Clear();

	const idDeclEntityDef *brassDef = gameLocal.FindEntityDef(weaponDef->dict.GetString("def_ejectBrass"), false);
	if(brassDef)
		brassDict = brassDef->dict;
	else
		brassDict.Clear();

	savefile->ReadFloat(meleeDistance);
	savefile->ReadString(meleeDefName);
	savefile->ReadInt(brassDelay);
	savefile->ReadString(icon);
	savefile->ReadString(pdaIcon);
	savefile->ReadString(displayName);
	savefile->ReadString(itemDesc);

	savefile->ReadInt(guiLightHandle);
	savefile->ReadRenderLight(guiLight);
	if(guiLightHandle >= 0)
		guiLightHandle = gameRenderWorld->AddLightDef(&guiLight);

	savefile->ReadInt(muzzleFlashHandle);
	savefile->ReadRenderLight(muzzleFlash);
	if(muzzleFlashHandle >= 0)
		muzzleFlashHandle = gameRenderWorld->AddLightDef(&muzzleFlash);

	savefile->ReadInt(worldMuzzleFlashHandle);
	savefile->ReadRenderLight(worldMuzzleFlash);
	if(worldMuzzleFlashHandle >= 0)
		worldMuzzleFlashHandle = gameRenderWorld->AddLightDef(&worldMuzzleFlash);

	savefile->ReadVec3(flashColor);
	savefile->ReadInt(muzzleFlashEnd);
	savefile->ReadInt(flashTime);

	savefile->ReadBool(lightOn);
	savefile->ReadBool(silent_fire);

	savefile->ReadInt(kick_endtime);
	savefile->ReadInt(muzzle_kick_time);
	savefile->ReadInt(muzzle_kick_maxtime);
	savefile->ReadAngles(muzzle_kick_angles);
	savefile->ReadVec3(muzzle_kick_offset);

	savefile->ReadInt((int &)ammoType);
	savefile->ReadInt(ammoRequired);
	savefile->ReadInt(clipSize);

	int savedAmmoClip = 0;
	savefile->ReadInt(savedAmmoClip);
	ammoClip = savedAmmoClip;

	savefile->ReadInt(lowAmmo);
	savefile->ReadBool(powerAmmo);

	// savegame versions <= 17
	int foo;
	savefile->ReadInt(foo);

	savefile->ReadInt(zoomFov);

	savefile->ReadJoint(barrelJointView);
	savefile->ReadJoint(flashJointView);
	savefile->ReadJoint(ejectJointView);
	savefile->ReadJoint(guiLightJointView);
	savefile->ReadJoint(ventLightJointView);

	savefile->ReadJoint(flashJointWorld);
	savefile->ReadJoint(barrelJointWorld);
	savefile->ReadJoint(ejectJointWorld);

	savefile->ReadBool(hasBloodSplat);

	savefile->ReadSoundShader(sndHum);

	savefile->ReadParticle(weaponSmoke);
	savefile->ReadInt(weaponSmokeStartTime);
	savefile->ReadBool(continuousSmoke);
	savefile->ReadParticle(strikeSmoke);
	savefile->ReadInt(strikeSmokeStartTime);
	savefile->ReadVec3(strikePos);
	savefile->ReadMat3(strikeAxis);
	savefile->ReadInt(nextStrikeFx);

	savefile->ReadBool(nozzleFx);
	savefile->ReadInt(nozzleFxFade);

	savefile->ReadInt(lastAttack);

	savefile->ReadInt(nozzleGlowHandle);
	savefile->ReadRenderLight(nozzleGlow);
	if(nozzleGlowHandle >= 0)
		nozzleGlowHandle = gameRenderWorld->AddLightDef(&nozzleGlow);

	savefile->ReadVec3(nozzleGlowColor);
	savefile->ReadMaterial(nozzleGlowShader);
	savefile->ReadFloat(nozzleGlowRadius);

	savefile->ReadInt(weaponAngleOffsetAverages);
	savefile->ReadFloat(weaponAngleOffsetScale);
	savefile->ReadFloat(weaponAngleOffsetMax);
	savefile->ReadFloat(weaponOffsetTime);
	savefile->ReadFloat(weaponOffsetScale);

	savefile->ReadBool(allowDrop);
	savefile->ReadObject(reinterpret_cast<idClass *&>(projectileEnt));

	savefile->ReadStaticObject(grabber);
	savefile->ReadInt(grabberState);

	savefile->ReadJoint(smokeJointView);

	int particleCount;
	savefile->ReadInt(particleCount);
	for(int i = 0; i < particleCount; i++)
	{
		WeaponParticle_t newParticle;
		memset(&newParticle, 0, sizeof(newParticle));

		idStr name, particlename;
		savefile->ReadString(name);
		savefile->ReadString(particlename);

		strcpy(newParticle.name, name.c_str());
		strcpy(newParticle.particlename, particlename.c_str());

		savefile->ReadBool(newParticle.active);
		savefile->ReadInt(newParticle.startTime);
		savefile->ReadJoint(newParticle.joint);
		savefile->ReadBool(newParticle.smoke);
		if(newParticle.smoke)
			newParticle.particle = static_cast<const idDeclParticle *>(declManager->FindType(DECL_PARTICLE, particlename, false));
		else
			savefile->ReadObject(reinterpret_cast<idClass *&>(newParticle.emitter));

		weaponParticles.Set(newParticle.name, newParticle);
	};

	int lightCount;
	savefile->ReadInt(lightCount);
	for(int i = 0; i < lightCount; i++)
	{
		WeaponLight_t newLight;
		memset(&newLight, 0, sizeof(newLight));

		idStr name;
		savefile->ReadString(name);
		strcpy(newLight.name, name.c_str());

		savefile->ReadBool(newLight.active);
		savefile->ReadInt(newLight.startTime);
		savefile->ReadJoint(newLight.joint);
		savefile->ReadInt(newLight.lightHandle);
		savefile->ReadRenderLight(newLight.light);
		if(newLight.lightHandle >= 0)
			newLight.lightHandle = gameRenderWorld->AddLightDef(&newLight.light);
		weaponLights.Set(newLight.name, newLight);
	};
};

/*
================
CBaseWeapon::SetOwner

Only called at player spawn time, not each weapon switch
================
*/
void CBaseWeapon::SetOwner(CBasePlayer *_owner)
{
	assert(!mpOwner);
	
	mpOwner = _owner;
	
	SetName(va("%s_weapon", mpOwner->name.c_str()));

	if(worldModel.GetEntity())
		worldModel.GetEntity()->SetName(va("%s_weapon_worldmodel", mpOwner->name.c_str()));
};

/*
================
CBaseWeapon::Raise
================
*/
void CBaseWeapon::Raise()
{
	if(isLinked)
		WEAPON_RAISEWEAPON = true;
};

/*
================
CBaseWeapon::PutAway
================
*/
void CBaseWeapon::PutAway()
{
	hasBloodSplat = false;
	if(isLinked)
		WEAPON_LOWERWEAPON = true;
};

/*
================
CBaseWeapon::Reload
NOTE: this is only for impulse-triggered reload, auto reload is scripted
================
*/
void CBaseWeapon::Reload()
{
	if(isLinked)
		WEAPON_RELOAD = true;
};

/*
================
CBaseWeapon::LowerWeapon
================
*/
void CBaseWeapon::LowerWeapon()
{
	if(!mbHide)
	{
		hideStart = 0.0f;
		hideEnd = hideDistance;

		if(gameLocal.time - hideStartTime < hideTime)
			hideStartTime = gameLocal.time - (hideTime - (gameLocal.time - hideStartTime));
		else
			hideStartTime = gameLocal.time;

		mbHide = true;
	};
};

/*
================
CBaseWeapon::RaiseWeapon
================
*/
void CBaseWeapon::RaiseWeapon()
{
	Show();

	if(mbHide)
	{
		hideStart = hideDistance;
		hideEnd = 0.0f;

		if(gameLocal.time - hideStartTime < hideTime)
			hideStartTime = gameLocal.time - (hideTime - (gameLocal.time - hideStartTime));
		else
			hideStartTime = gameLocal.time;

		mbHide = false;
	};
};

/*
================
CBaseWeapon::HideWeapon
================
*/
void CBaseWeapon::HideWeapon()
{
	Hide();

	if(worldModel.GetEntity())
		worldModel.GetEntity()->Hide();

	muzzleFlashEnd = 0;
};

/*
================
CBaseWeapon::ShowWeapon
================
*/
void CBaseWeapon::ShowWeapon()
{
	Show();

	if(worldModel.GetEntity())
		worldModel.GetEntity()->Show();

	if(lightOn)
		MuzzleFlashLight();
};

/*
================
CBaseWeapon::BeginAttack
================
*/
void CBaseWeapon::BeginAttack()
{
	if(status != WP_OUTOFAMMO)
		lastAttack = gameLocal.time;

	if(!isLinked)
		return;

	if(!WEAPON_ATTACK)
		if(sndHum && grabberState == -1) // _D3XP :: don't stop grabber hum
			StopSound(SND_CHANNEL_BODY, false);

	WEAPON_ATTACK = true;
};

/*
================
CBaseWeapon::EndAttack
================
*/
void CBaseWeapon::EndAttack()
{
	if(!WEAPON_ATTACK.IsLinked())
		return;

	if(WEAPON_ATTACK)
	{
		WEAPON_ATTACK = false;
		if(sndHum && grabberState == -1) // _D3XP :: don't stop grabber hum
			StartSoundShader(sndHum, SND_CHANNEL_BODY, 0, false, nullptr);
	};
};

/*
================
CBaseWeapon::BeginSecondaryAttack
================
*/
void CBaseWeapon::BeginSecondaryAttack()
{
	if(status != WP_OUTOFAMMO)
		lastAttack = gameLocal.time;

	if(!isLinked)
		return;

	if(!WEAPON_ATTACK2)
		if(sndHum && grabberState == -1) // _D3XP :: don't stop grabber hum
			StopSound(SND_CHANNEL_BODY, false);

	WEAPON_ATTACK2 = true;
};

/*
================
CBaseWeapon::EndSecondaryAttack
================
*/
void CBaseWeapon::EndSecondaryAttack()
{
	if(!WEAPON_ATTACK2.IsLinked())
		return;

	if(WEAPON_ATTACK2)
	{
		WEAPON_ATTACK2 = false;
		if(sndHum && grabberState == -1) // _D3XP :: don't stop grabber hum
			StartSoundShader(sndHum, SND_CHANNEL_BODY, 0, false, nullptr);
	};
};

/*
================
CBaseWeapon::HideWorldModel
================
*/
void CBaseWeapon::HideWorldModel()
{
	if(worldModel.GetEntity())
		worldModel.GetEntity()->Hide();
};

/*
================
CBaseWeapon::ShowWorldModel
================
*/
void CBaseWeapon::ShowWorldModel()
{
	if(worldModel.GetEntity())
		worldModel.GetEntity()->Show();
};

/*
=====================
CBaseWeapon::CanDrop
=====================
*/
bool CBaseWeapon::CanDrop() const
{
	if(!weaponDef || !worldModel.GetEntity())
		return false;
	
	const char *classname = weaponDef->dict.GetString("def_dropItem");
	if(!classname[0])
		return false;
	
	return true;
};

/*
================
CBaseWeapon::IsReloading
================
*/
bool CBaseWeapon::IsReloading() const
{
	return status == WP_RELOAD;
};

/*
================
CBaseWeapon::IsHolstered
================
*/
bool CBaseWeapon::IsHolstered() const
{
	return status == WP_HOLSTERED;
};

/*
================
CBaseWeapon::GetAmmoType
================
*/
ammo_t CBaseWeapon::GetAmmoType() const
{
	return meAmmoType;
};

/*
================
CBaseWeapon::GetSecondaryAmmoType
================
*/
ammo_t CBaseWeapon::GetSecondaryAmmoType() const
{
	return meSecondaryAmmoType;
};

/*
================
CBaseWeapon::ClipSize
================
*/
int CBaseWeapon::ClipSize() const
{
	return mnClipSize;
};

/*
================
CBaseWeapon::SecondaryClipSize
================
*/
int CBaseWeapon::SecondaryClipSize() const
{
	return mnSecondaryClipSize;
};

/*
================
CBaseWeapon::AmmoInClip
================
*/
int CBaseWeapon::AmmoInClip() const
{
	return mAmmoClip.Get();
};

/*
================
CBaseWeapon::AmmoInSecondaryClip
================
*/
int CBaseWeapon::AmmoInSecondaryClip() const
{
	return mSecondaryAmmoClip.Get();
};

/*
================
CBaseWeapon::AmmoCount

Returns the total number of rounds regardless of the required ammo
================
*/
int CBaseWeapon::AmmoCount() const
{
	if(owner)
		return owner->inventory.HasAmmo(ammoType, 1);
	
	return 0;
};

/*
================
CBaseWeapon::ResetAmmoClip
================
*/
void CBaseWeapon::ResetAmmoClip()
{
	mnAmmoClip = -1;
};

/*
================
CBaseWeapon::ResetSecondaryAmmoClip
================
*/
void CBaseWeapon::ResetSecondaryAmmoClip()
{
	mbSecondaryAmmoClip = -1;
};

/*
================
CBaseWeapon::AmmoAvailable
================
*/
int CBaseWeapon::AmmoAvailable() const
{
	if(owner)
		return owner->inventory.HasAmmo(meAmmoType, ammoRequired);
	else
	{
		if(g_infiniteAmmo.GetBool())
			return 999; // arbitrary number, just so whatever's calling thinks there's sufficient ammo...
		else
			return 0;
	};
};

/*
================
CBaseWeapon::SecondaryAmmoAvailable
================
*/
int CBaseWeapon::SecondaryAmmoAvailable() const
{
	if(owner)
		return owner->inventory.HasAmmo(meSecondaryAmmoType, ammoRequired);
	else
	{
		if(g_infiniteAmmo.GetBool())
			return 999; // arbitrary number, just so whatever's calling thinks there's sufficient ammo...
		else
			return 0;
	};
};

/*
================
CBaseWeapon::LowAmmo
================
*/
int CBaseWeapon::LowAmmo() const
{
	return lowAmmo;
};

/*
================
CBaseWeapon::LowSecondaryAmmo
================
*/
int CBaseWeapon::LowSecondaryAmmo() const
{
	return lowSecondaryAmmo;
};

/*
================
CBaseWeapon::AmmoRequired
================
*/
int CBaseWeapon::AmmoRequired() const
{
	return mbAmmoRequired;
};

/*
================
CBaseWeapon::SecondaryAmmoRequired
================
*/
int CBaseWeapon::SecondaryAmmoRequired() const
{
	return mbSecondaryAmmoRequired;
};

/*
================
CBaseWeapon::SecondaryAmmoCount

Returns the total number of rounds regardless of the required ammo
================
*/
int CBaseWeapon::SecondaryAmmoCount() const
{
	if(owner)
		return owner->inventory.HasAmmo(ammoType, 1); // TODO
	
	return 0;
};

/*
================
CBaseWeapon::AlertMonsters
================
*/
void CBaseWeapon::AlertMonsters()
{
	trace_t tr;
	idEntity *ent;
	idVec3 end = muzzleFlash.origin + muzzleFlash.axis * muzzleFlash.target;

	gameLocal.clip.TracePoint(tr, muzzleFlash.origin, end, CONTENTS_OPAQUE | MASK_SHOT_RENDERMODEL | CONTENTS_FLASHLIGHT_TRIGGER, owner);
	if(g_debugWeapon.GetBool())
	{
		gameRenderWorld->DebugLine(colorYellow, muzzleFlash.origin, end, 0);
		gameRenderWorld->DebugArrow(colorGreen, muzzleFlash.origin, tr.endpos, 2, 0);
	}

	if(tr.fraction < 1.0f)
	{
		ent = gameLocal.GetTraceEntity(tr);
		if(ent->IsType(idAI::Type))
		{
			static_cast<idAI *>(ent)->TouchedByFlashlight(owner);
		}
		else if(ent->IsType(idTrigger::Type))
		{
			ent->Signal(SIG_TOUCH);
			ent->ProcessEvent(&EV_Touch, owner, &tr);
		};
	};

	// jitter the trace to try to catch cases where a trace down the center doesn't hit the monster
	end += muzzleFlash.axis * muzzleFlash.right * idMath::Sin16(MS2SEC(gameLocal.time) * 31.34f);
	end += muzzleFlash.axis * muzzleFlash.up * idMath::Sin16(MS2SEC(gameLocal.time) * 12.17f);
	gameLocal.clip.TracePoint(tr, muzzleFlash.origin, end, CONTENTS_OPAQUE | MASK_SHOT_RENDERMODEL | CONTENTS_FLASHLIGHT_TRIGGER, owner);
	if(g_debugWeapon.GetBool())
	{
		gameRenderWorld->DebugLine(colorYellow, muzzleFlash.origin, end, 0);
		gameRenderWorld->DebugArrow(colorGreen, muzzleFlash.origin, tr.endpos, 2, 0);
	};

	if(tr.fraction < 1.0f)
	{
		ent = gameLocal.GetTraceEntity(tr);
		if(ent->IsType(idAI::Type))
			static_cast<idAI *>(ent)->TouchedByFlashlight(owner);
		else if(ent->IsType(idTrigger::Type))
		{
			ent->Signal(SIG_TOUCH);
			ent->ProcessEvent(&EV_Touch, owner, &tr);
		};
	};
};

/***********************************************************************

	GUIs

***********************************************************************/

/*
================
CBaseWeapon::Icon
================
*/
const char *CBaseWeapon::Icon() const
{
	return icon;
}

/*
================
CBaseWeapon::PdaIcon
================
*/
const char *CBaseWeapon::PdaIcon() const
{
	return pdaIcon;
}

/*
================
CBaseWeapon::DisplayName
================
*/
const char *CBaseWeapon::DisplayName() const
{
	return idLocalization::GetString(displayName);
}

/*
================
CBaseWeapon::Description
================
*/
const char *CBaseWeapon::Description() const
{
	return idLocalization::GetString(itemDesc);
};

/*
================
CBaseWeapon::FlashlightOn
================
*/
void CBaseWeapon::FlashlightOn()
{
	const function_t *func;

	if(!isLinked)
		return;

	func = scriptObject.GetFunction("TurnOn");
	if(!func)
	{
		common->Warning("Can't find function 'TurnOn' in object '%s'", scriptObject.GetTypeName());
		return;
	};

	// use the frameCommandThread since it's safe to use outside of framecommands
	gameLocal.frameCommandThread->CallFunction(this, func, true);
	gameLocal.frameCommandThread->Execute();

	return;
};

/*
================
CBaseWeapon::FlashlightOff
================
*/
void CBaseWeapon::FlashlightOff()
{
	const function_t *func;

	if(!isLinked)
		return;

	func = scriptObject.GetFunction("TurnOff");
	if(!func)
	{
		common->Warning("Can't find function 'TurnOff' in object '%s'", scriptObject.GetTypeName());
		return;
	};

	// use the frameCommandThread since it's safe to use outside of framecommands
	gameLocal.frameCommandThread->CallFunction(this, func, true);
	gameLocal.frameCommandThread->Execute();

	return;
};

/*
================
CBaseWeapon::OwnerDied
================
*/
void CBaseWeapon::OwnerDied()
{
	if(isLinked)
	{
		SetState("OwnerDied", 0);
		thread->Execute();

		// Update the grabber effects
		if(/*!common->IsMultiplayer() &&*/ grabberState != -1)
			grabber.Update(owner, hide);
	};

	Hide();
	
	if(worldModel.GetEntity())
		worldModel.GetEntity()->Hide();

	// don't clear the weapon immediately since the owner might have killed himself by firing the weapon
	// within the current stack frame
	PostEventMS(&EV_Weapon_Clear, 0);
};

/***********************************************************************

	Script state management

***********************************************************************/

/*
=====================
CBaseWeapon::SetState
=====================
*/
void CBaseWeapon::SetState(const char *statename, int blendFrames)
{
	const function_t *func;

	if(!isLinked)
		return;

	func = scriptObject.GetFunction(statename);
	if(!func)
	{
		assert(0);
		gameLocal.Error("Can't find function '%s' in object '%s'", statename, scriptObject.GetTypeName());
	};

	thread->CallFunction(this, func, true);
	state = statename;

	animBlendFrames = blendFrames;
	if(g_debugWeapon.GetBool())
		gameLocal.Printf("%d: weapon state : %s\n", gameLocal.time, statename);

	idealState = "";
};

/*
================
CBaseWeapon::ConstructScriptObject

Called during idEntity::Spawn.  Calls the constructor on the script object.
Can be overridden by subclasses when a thread doesn't need to be allocated.
================
*/
idThread *CBaseWeapon::ConstructScriptObject()
{
	const function_t *constructor;

	thread->EndThread();

	// call script object's constructor
	constructor = scriptObject.GetConstructor();
	if(!constructor)
		gameLocal.Error("Missing constructor on '%s' for weapon", scriptObject.GetTypeName());

	// init the script object's data
	scriptObject.ClearObject();
	thread->CallFunction(this, constructor, true);
	thread->Execute();

	return thread;
};

/*
================
CBaseWeapon::DeconstructScriptObject

Called during CBaseEntity::~CBaseEntity.  Calls the destructor on the script object.
Can be overridden by subclasses when a thread doesn't need to be allocated.
Not called during idGameLocal::MapShutdown.
================
*/
void CBaseWeapon::DeconstructScriptObject()
{
	const function_t *destructor;

	if(!thread)
		return;

	// don't bother calling the script object's destructor on map shutdown
	if(gameLocal.GameState() == GAMESTATE_SHUTDOWN)
		return;

	thread->EndThread();

	// call script object's destructor
	destructor = scriptObject.GetDestructor();
	if(destructor)
	{
		// start a thread that will run immediately and end
		thread->CallFunction(this, destructor, true);
		thread->Execute();
		thread->EndThread();
	};

	// clear out the object's memory
	scriptObject.ClearObject();
};

/*
================
CBaseWeapon::UpdateScript
================
*/
void CBaseWeapon::UpdateScript()
{
	int count;

	if(!isLinked)
		return;

	// only update the script on new frames
	if(!gameLocal.isNewFrame)
		return;

	if(idealState.Length())
		SetState(idealState, animBlendFrames);

	// update script state, which may call Event_LaunchProjectiles, among other things
	count = 10;
	while((thread->Execute() || idealState.Length()) && count--)
	{
		// happens for weapons with no clip (like grenades)
		if(idealState.Length())
			SetState(idealState, animBlendFrames);
	};

	WEAPON_RELOAD = false;
};

/*
===============
CBaseWeapon::Event_NetReload
===============
*/
void CBaseWeapon::Event_NetReload()
{
	assert(owner);
	
	ServerSendEvent(EVENT_RELOAD, nullptr, false);
};

/*
===============
CBaseWeapon::Event_NetEndReload
===============
*/
void CBaseWeapon::Event_NetEndReload()
{
	assert(owner);
	
	ServerSendEvent(EVENT_ENDRELOAD, nullptr, false);
};

//q2
/*
================
Weapon_Generic

A generic function to handle the basics of weapon thinking
================
*/
#define FRAME_FIRE_FIRST (FRAME_ACTIVATE_LAST + 1)
#define FRAME_IDLE_FIRST (FRAME_FIRE_LAST + 1)
#define FRAME_DEACTIVATE_FIRST (FRAME_IDLE_LAST + 1)

void Weapon_Generic(edict_t *ent, int FRAME_ACTIVATE_LAST, int FRAME_FIRE_LAST, int FRAME_IDLE_LAST, int FRAME_DEACTIVATE_LAST, int *pause_frames, int *fire_frames, void (*fire)(edict_t *ent))
{
	int n;

	if(ent->deadflag || ent->s.modelindex != 255) // VWep animations screw up corpses
	{
		return;
	}

	if(ent->client->weaponstate == WEAPON_DROPPING)
	{
		if(ent->client->ps.gunframe == FRAME_DEACTIVATE_LAST)
		{
			ChangeWeapon(ent);
			return;
		}
		else if((FRAME_DEACTIVATE_LAST - ent->client->ps.gunframe) == 4)
		{
			ent->client->anim_priority = ANIM_REVERSE;
			if(ent->client->ps.pmove.pm_flags & PMF_DUCKED)
			{
				ent->s.frame = FRAME_crpain4 + 1;
				ent->client->anim_end = FRAME_crpain1;
			}
			else
			{
				ent->s.frame = FRAME_pain304 + 1;
				ent->client->anim_end = FRAME_pain301;
			}
		}

		ent->client->ps.gunframe++;
		return;
	}

	if(ent->client->weaponstate == WEAPON_ACTIVATING)
	{
		if(ent->client->ps.gunframe == FRAME_ACTIVATE_LAST)
		{
			ent->client->weaponstate = WEAPON_READY;
			ent->client->ps.gunframe = FRAME_IDLE_FIRST;
			return;
		}

		ent->client->ps.gunframe++;
		return;
	}

	if((ent->client->newweapon) && (ent->client->weaponstate != WEAPON_FIRING))
	{
		ent->client->weaponstate = WEAPON_DROPPING;
		ent->client->ps.gunframe = FRAME_DEACTIVATE_FIRST;

		if((FRAME_DEACTIVATE_LAST - FRAME_DEACTIVATE_FIRST) < 4)
		{
			ent->client->anim_priority = ANIM_REVERSE;
			if(ent->client->ps.pmove.pm_flags & PMF_DUCKED)
			{
				ent->s.frame = FRAME_crpain4 + 1;
				ent->client->anim_end = FRAME_crpain1;
			}
			else
			{
				ent->s.frame = FRAME_pain304 + 1;
				ent->client->anim_end = FRAME_pain301;
			}
		}
		return;
	}

	if(ent->client->weaponstate == WEAPON_READY)
	{
		if(((ent->client->latched_buttons | ent->client->buttons) & BUTTON_ATTACK))
		{
			ent->client->latched_buttons &= ~BUTTON_ATTACK;
			if((!ent->client->ammo_index) ||
			   (ent->client->pers.inventory[ent->client->ammo_index] >= ent->client->pers.weapon->quantity))
			{
				ent->client->ps.gunframe = FRAME_FIRE_FIRST;
				ent->client->weaponstate = WEAPON_FIRING;

				// start the animation
				ent->client->anim_priority = ANIM_ATTACK;
				if(ent->client->ps.pmove.pm_flags & PMF_DUCKED)
				{
					ent->s.frame = FRAME_crattak1 - 1;
					ent->client->anim_end = FRAME_crattak9;
				}
				else
				{
					ent->s.frame = FRAME_attack1 - 1;
					ent->client->anim_end = FRAME_attack8;
				}
			}
			else
			{
				if(level.time >= ent->pain_debounce_time)
				{
					ent->EmitSound(CHAN_VOICE, "weapons/noammo.wav", 1, ATTN_NORM, 0);
					ent->pain_debounce_time = level.time + 1;
				};
				NoAmmoWeaponChange(ent);
			};
		}
		else
		{
			if(ent->client->ps.gunframe == FRAME_IDLE_LAST)
			{
				ent->client->ps.gunframe = FRAME_IDLE_FIRST;
				return;
			}

			if(pause_frames)
			{
				for(n = 0; pause_frames[n]; n++)
				{
					if(ent->client->ps.gunframe == pause_frames[n])
					{
						if(rand() & 15)
							return;
					}
				}
			}

			ent->client->ps.gunframe++;
			return;
		}
	}

	if(ent->client->weaponstate == WEAPON_FIRING)
	{
		for(n = 0; fire_frames[n]; n++)
		{
			if(ent->client->ps.gunframe == fire_frames[n])
			{
				if(ent->client->quad_framenum > level.framenum)
					gi.sound(ent, CHAN_ITEM, gi.soundindex("items/damage3.wav"), 1, ATTN_NORM, 0);

				fire(ent);
				break;
			}
		}

		if(!fire_frames[n])
			ent->client->ps.gunframe++;

		if(ent->client->ps.gunframe == FRAME_IDLE_FIRST + 1)
			ent->client->weaponstate = WEAPON_READY;
	}
}