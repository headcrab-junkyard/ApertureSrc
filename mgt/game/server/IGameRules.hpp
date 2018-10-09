/// @file
/// @brief game rules interface

#pragma once

class CBasePlayer;

struct IGameRules
{
	///
	virtual void OnGameStart() = 0;

	///
	virtual void OnGameEnd() = 0;

	///
	virtual void OnPlayerSpawn(CBasePlayer *apPlayer) = 0;

	///
	virtual void OnPlayerDeath(CBasePlayer *apPlayer) = 0;
	
	///
	virtual bool IsMultiplayer() const = 0; //{return false;}
	
	///
	virtual bool IsCoOp() const = 0; //{return false;} // TODO: IsCoop?
	
	///
	virtual bool IsDeathmatch() const = 0;
	
	///
	virtual bool IsTeamplay() const = 0;
};