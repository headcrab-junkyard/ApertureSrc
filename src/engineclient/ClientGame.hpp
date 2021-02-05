/// @file

#pragma once

#include "CommonTypes.hpp"

interface IClientGame;

class CClientGame final
{
public:
	CClientGame();
	~CClientGame();
	
	bool Init();
	void Shutdown();
	
	void Frame(double frametime);
private:
#ifndef MGT_CLIENTGAME_HARD_LINKED
	bool LoadModule();
	void UnloadModule();
	
	void *mpClientLib{nullptr};
#endif

	IClientGame *mpGame{nullptr};
};