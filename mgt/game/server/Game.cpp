/// @file

#include "Game.hpp"

IConsoleManager *gpConsoleManager{nullptr};

void DoABarrelRoll_f(const ICmdArgs &aArgs)
{
	static int nRolls{1};
	
	if(aArgs.GetCount() == 2)
		nRolls = atoi(aArgs.GetByIndex(2));
	
	for(auto i = 0; i < nCount; ++i)
		printf("Barrel roll!\n");
	
	printf("Do a barrel roll!\n");
};

EXPOSE_SINGLE_INTERFACE(CGame, IGame, MGT_GAME_INTERFACE_VERSION);

CGame::CGame() = default;
CGame::~CGame() = default;

bool CGame::Init(CreateInterfaceFn afnEngineFactory)
{
	// Put your init routines here...
	
	gpConsoleManager = (IConsoleManager*)afnEngineFactory(MGT_CONSOLEMANAGER_INTERFACE_VERSION, nullptr);
	
	if(!gpConsoleManager)
		return false;
	
	CConCmd Cmd_DoABarrelRoll("barrelroll", DoABarrelRoll_f, FCVAR_EXTDLL, "Does a barrel roll (kinda)");
	
	if(!gpConsoleManager->RegisterCmd(Cmd_DoABarrelRoll))
		return false;
	
	// Something game-specific...
	
	return true;
};

void CGame::Shutdown()
{
	// Put your shutdown routines here...
};

void CGame::Update()
{
	// Per-frame logic update
};