//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: Main.cpp
//===============================================================================

#include "SDK/SDK.h"
#include "Functions.h"

#include "CPawnManager.h"
#include "CPawnCallbacks.h"
#include "CPawnNatives.h"

CPawnManager * g_pPawnManager = NULL;

const char * m_szModuleName = "IV:MP modPawn";

AMX_NATIVE_INFO pawn_Natives[] = 
{
	// Player
	{ "IsPlayerConnected", CPawnNatives::IsPlayerConnected },
	{ "SetPlayerHealth", CPawnNatives::SetPlayerHealth },
	{ "GetPlayerHealth", CPawnNatives::GetPlayerHealth },
	{ "SetPlayerCoordinates", CPawnNatives::SetPlayerCoordinates },
	{ "GetPlayerCoordinates", CPawnNatives::GetPlayerCoordinates },
	{ "SetPlayerHeading", CPawnNatives::SetPlayerHeading },
	{ "GetPlayerHeading", CPawnNatives::GetPlayerHeading },
	{ "SetPlayerModel", CPawnNatives::SetPlayerModel },
	{ "GetPlayerModel", CPawnNatives::GetPlayerModel },
	{ "SetPlayerTime", CPawnNatives::SetPlayerTime },
	{ "SetPlayerGravity", CPawnNatives::SetPlayerGravity },
	{ "SendPlayerMessage", CPawnNatives::SendPlayerMessage },
	{ "GetPlayerVehicleId", CPawnNatives::GetPlayerVehicleId },
	{ "IsPlayerOnFoot", CPawnNatives::IsPlayerOnFoot },
	{ "IsPlayerInVehicle", CPawnNatives::IsPlayerInVehicle },
	{ "IsPlayerSpawned", CPawnNatives::IsPlayerSpawned },
	{ "SetPlayerArmour", CPawnNatives::SetPlayerArmour },
	{ "GetPlayerArmour", CPawnNatives::GetPlayerArmour },
	{ "SetPlayerMoney", CPawnNatives::SetPlayerMoney },
	{ "GetPlayerMoney", CPawnNatives::GetPlayerMoney },
	{ "GivePlayerMoney", CPawnNatives::GivePlayerMoney },
	{ "ResetPlayerMoney", CPawnNatives::ResetPlayerMoney },
	{ "SetPlayerVelocity", CPawnNatives::SetPlayerVelocity },
	{ "GetPlayerVelocity", CPawnNatives::GetPlayerVelocity },
	{ "GetPlayerState", CPawnNatives::GetPlayerState },
	{ "SetPlayerWantedLevel", CPawnNatives::SetPlayerWantedLevel },
	{ "IsPlayerInAnyVehicle", CPawnNatives::IsPlayerInAnyVehicle },
	{ "WarpPlayerIntoVehicle", CPawnNatives::WarpPlayerIntoVehicle },
	{ "RemovePlayerFromVehicle", CPawnNatives::RemovePlayerFromVehicle },
	{ "GivePlayerWeapon", CPawnNatives::GivePlayerWeapon },
	{ "RemovePlayerWeapons", CPawnNatives::RemovePlayerWeapons },
	{ "GetPlayerAmmo", CPawnNatives::GetPlayerAmmo },
	{ "GetPlayerWeapon", CPawnNatives::GetPlayerWeapon },
	{ "SetCameraBehindPlayer", CPawnNatives::SetCameraBehindPlayer },
	{ "GetPlayerSeatId", CPawnNatives::GetPlayerSeatId },
	{ "SetPlayerInterior", CPawnNatives::SetPlayerInterior },
	{ "GetPlayerInterior", CPawnNatives::GetPlayerInterior },
	{ "SetPlayerWeather", CPawnNatives::SetPlayerWeather },
	{ "SetPlayerColor", CPawnNatives::SetPlayerColor },
	{ "GetPlayerColor", CPawnNatives::GetPlayerColor },
	{ "SetPlayerName", CPawnNatives::SetPlayerName },
	{ "GetPlayerPing", CPawnNatives::GetPlayerPing },
	{ "SetPlayerClothes", CPawnNatives::SetPlayerClothes },
	{ "GetPlayerClothes", CPawnNatives::GetPlayerClothes },
	{ "ResetPlayerClothes", CPawnNatives::ResetPlayerClothes },


	{ NULL, NULL }
};

EXPORT bool InitModule(char * szModuleName)
{
	strcpy(szModuleName, "Test Module");

	LogPrintf("[%s] Our module got loaded!", m_szModuleName);
	LogPrintf("[%s] Loading callbacks...", m_szModuleName);
	GetEventManager()->AddModuleEvent("playerConnect", CPawnCallbacks::OnPlayerConnect);
	GetEventManager()->AddModuleEvent("playerDisconnect", CPawnCallbacks::OnPlayerDisconnect);
	GetEventManager()->AddModuleEvent("playerNameCheck", CPawnCallbacks::OnPlayerNameCheck);
	GetEventManager()->AddModuleEvent("playerText", CPawnCallbacks::OnPlayerText);
	GetEventManager()->AddModuleEvent("playerCommand", CPawnCallbacks::OnPlayerCommand);
	GetEventManager()->AddModuleEvent("playerSpawn", CPawnCallbacks::OnPlayerSpawn);
	GetEventManager()->AddModuleEvent("playerDeath", CPawnCallbacks::OnPlayerDeath);
	GetEventManager()->AddModuleEvent("playerEnterVehicle", CPawnCallbacks::OnPlayerEnterVehicle);
	GetEventManager()->AddModuleEvent("playerCancelVehicleEntry", CPawnCallbacks::OnPlayerCancelVehicleEntry);
	GetEventManager()->AddModuleEvent("playerChangeState", CPawnCallbacks::OnPlayerChangeState);
	GetEventManager()->AddModuleEvent("playerChangePadState", CPawnCallbacks::OnPlayerChangePadState);

	GetEventManager()->AddModuleEvent("vehicleCreate", CPawnCallbacks::OnVehicleCreate);
	GetEventManager()->AddModuleEvent("vehicleDelete", CPawnCallbacks::OnVehicleDelete);

	GetEventManager()->AddModuleEvent("objectCreate", CPawnCallbacks::OnObjectCreate);
	GetEventManager()->AddModuleEvent("objectDelete", CPawnCallbacks::OnObjectDelete);

	GetEventManager()->AddModuleEvent("playerEnterCheckpoint", CPawnCallbacks::OnPlayerEnterCheckpoint);
	GetEventManager()->AddModuleEvent("playerLeaveCheckpoint", CPawnCallbacks::OnPlayerLeaveCheckpoint);

	GetEventManager()->AddModuleEvent("blipCreate", CPawnCallbacks::OnBlipCreate);
	GetEventManager()->AddModuleEvent("blipDelete", CPawnCallbacks::OnBlipDelete);

	GetEventManager()->AddModuleEvent("actorCreate", CPawnCallbacks::OnActorCreate);
	GetEventManager()->AddModuleEvent("actorDelete", CPawnCallbacks::OnActorDelete);

	LogPrintf("[%s] Callbacks loaded. Loading scripts...", m_szModuleName);

	g_pPawnManager = new CPawnManager();

	g_pPawnManager->Load("test.amx");

	return true;
}

EXPORT void ScriptLoad(HSQUIRRELVM pVM)
{

}

EXPORT void ScriptUnload(HSQUIRRELVM pVM)
{
	LogPrintf("[%s] A script got unloaded!", m_szModuleName);
}

EXPORT void Pulse()
{
	
}