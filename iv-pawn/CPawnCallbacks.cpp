
#include "SDK/SDK.h"
#include "CPawnCallbacks.h"

void CPawnCallbacks::OnPlayerConnect(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerConnect", 1, "i", pArguments->Get(0)->GetInteger());
}

void CPawnCallbacks::OnPlayerDisconnect(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerDisconnect", 1, "i", pArguments->Get(0)->GetInteger());
}	

void CPawnCallbacks::OnPlayerNameCheck(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerNameCheck", 1, "is", pArguments->Get(0)->GetInteger(), pArguments->Get(1)->GetString());
}

void CPawnCallbacks::OnPlayerText(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerText", 1, "is", pArguments->Get(0)->GetInteger(), pArguments->Get(1)->GetString());
}

void CPawnCallbacks::OnPlayerCommand(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerCommand", 1, "is", pArguments->Get(0)->GetInteger(), pArguments->Get(1)->GetString());
}

void CPawnCallbacks::OnPlayerSpawn(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerSpawn", 1, "i", pArguments->Get(0)->GetInteger());
}

void CPawnCallbacks::OnPlayerDeath(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerDeath", 1, "i", pArguments->Get(0)->GetInteger(), pArguments->Get(1)->GetInteger(), pArguments->Get(2)->GetInteger());
}

void CPawnCallbacks::OnPlayerEnterVehicle(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerEnterVehicle", 1, "iii", pArguments->Get(0)->GetInteger(), pArguments->Get(1)->GetInteger(), pArguments->Get(2)->GetInteger());
}

void CPawnCallbacks::OnPlayerCancelVehicleEntry(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerCancelVehicleEntry", 1, "i", pArguments->Get(0)->GetInteger());
}

void CPawnCallbacks::OnPlayerChangeState(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerChangeState", 1, "i", pArguments->Get(0)->GetInteger(), pArguments->Get(1)->GetInteger(), pArguments->Get(2)->GetInteger());
}

void CPawnCallbacks::OnPlayerChangePadState(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerChangePadState", 1, "i", pArguments->Get(0)->GetInteger());
}

void CPawnCallbacks::OnVehicleCreate(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnVehicleCreate", 1, "i", pArguments->Get(0)->GetInteger());
}

void CPawnCallbacks::OnVehicleDelete(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnVehicleDelete", 1, "i", pArguments->Get(0)->GetInteger());
}

void CPawnCallbacks::OnObjectCreate(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnObjectCreate", 1, "i", pArguments->Get(0)->GetInteger());
}

void CPawnCallbacks::OnObjectDelete(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnObjectDelete", 1, "i", pArguments->Get(0)->GetInteger());
}

void CPawnCallbacks::OnPlayerEnterCheckpoint(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerEnterCheckpoint", 1, "ii", pArguments->Get(0)->GetInteger(), pArguments->Get(1)->GetInteger());
}

void CPawnCallbacks::OnPlayerLeaveCheckpoint(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnPlayerLeaveCheckpoint", 1, "ii", pArguments->Get(0)->GetInteger(), pArguments->Get(1)->GetInteger());
}

void CPawnCallbacks::OnBlipCreate(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnBlipCreate", 1, "i", pArguments->Get(0)->GetInteger());
}

void CPawnCallbacks::OnBlipDelete(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnBlipDelete", 1, "i", pArguments->Get(0)->GetInteger());
}

void CPawnCallbacks::OnActorCreate(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnActorCreate", 1, "i", pArguments->Get(0)->GetInteger());
}

void CPawnCallbacks::OnActorDelete(SquirrelArgumentsInterface *pArguments, SquirrelArgumentInterface *pReturn, void *pChunk)
{
	g_pPawnManager->CallFunction("OnActorDelete", 1, "i", pArguments->Get(0)->GetInteger());
}