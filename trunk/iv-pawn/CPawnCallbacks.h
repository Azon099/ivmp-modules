#pragma once

#include "CPawnManager.h"

class CPawnCallbacks
{
public:
	static void OnPlayerConnect(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnPlayerDisconnect(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnPlayerNameCheck(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnPlayerText(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnPlayerCommand(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnPlayerSpawn(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnPlayerDeath(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnPlayerEnterVehicle(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnPlayerCancelVehicleEntry(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnPlayerChangeState(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnPlayerChangePadState(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);

	static void OnVehicleCreate(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnVehicleDelete(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);

	static void OnObjectCreate(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnObjectDelete(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);

	static void OnPlayerEnterCheckpoint(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnPlayerLeaveCheckpoint(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);

	static void OnBlipCreate(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnBlipDelete(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);

	static void OnActorCreate(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
	static void OnActorDelete(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);
};