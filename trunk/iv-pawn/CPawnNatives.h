#pragma once

#include "CPawnManager.h"

#define NATIVE cell AMX_NATIVE_CALL

class CPawnNatives
{
public:
	static NATIVE IsPlayerConnected(AMX *amx,const cell *params);
	static NATIVE SetPlayerHealth(AMX *amx,const cell *params);
	static NATIVE GetPlayerHealth(AMX *amx,const cell *params);
	static NATIVE SetPlayerCoordinates(AMX *amx,const cell *params);
	static NATIVE GetPlayerCoordinates(AMX *amx,const cell *params);
	static NATIVE SetPlayerHeading(AMX *amx,const cell *params);
	static NATIVE GetPlayerHeading(AMX *amx,const cell *params);
	static NATIVE SetPlayerModel(AMX *amx,const cell *params);
	static NATIVE GetPlayerModel(AMX *amx,const cell *params);
	static NATIVE SetPlayerTime(AMX *amx,const cell *params);
	static NATIVE SetPlayerGravity(AMX *amx,const cell *params);
	static NATIVE SendPlayerMessage(AMX *amx,const cell *params);
	static NATIVE GetPlayerVehicleId(AMX *amx,const cell *params);
	static NATIVE IsPlayerOnFoot(AMX *amx,const cell *params);
	static NATIVE IsPlayerInVehicle(AMX *amx,const cell *params);
	static NATIVE IsPlayerSpawned(AMX *amx,const cell *params);
	static NATIVE SetPlayerArmour(AMX *amx,const cell *params);
	static NATIVE GetPlayerArmour(AMX *amx,const cell *params);
	static NATIVE SetPlayerMoney(AMX *amx,const cell *params);
	static NATIVE GetPlayerMoney(AMX *amx,const cell *params);
	static NATIVE GivePlayerMoney(AMX *amx,const cell *params);
	static NATIVE ResetPlayerMoney(AMX *amx,const cell *params);
	static NATIVE SetPlayerVelocity(AMX *amx,const cell *params);
	static NATIVE GetPlayerVelocity(AMX *amx,const cell *params);
	static NATIVE GetPlayerState(AMX *amx,const cell *params);
	static NATIVE SetPlayerWantedLevel(AMX *amx,const cell *params);
	static NATIVE IsPlayerInAnyVehicle(AMX *amx,const cell *params);
	static NATIVE WarpPlayerIntoVehicle(AMX *amx,const cell *params);
	static NATIVE RemovePlayerFromVehicle(AMX *amx,const cell *params);
	static NATIVE GivePlayerWeapon(AMX *amx,const cell *params);
	static NATIVE RemovePlayerWeapons(AMX *amx,const cell *params);
	static NATIVE GetPlayerAmmo(AMX *amx,const cell *params);
	static NATIVE GetPlayerWeapon(AMX *amx,const cell *params);
	static NATIVE SetCameraBehindPlayer(AMX *amx,const cell *params);
	static NATIVE GetPlayerSeatId(AMX *amx,const cell *params);
	static NATIVE SetPlayerInterior(AMX *amx,const cell *params);
	static NATIVE GetPlayerInterior(AMX *amx,const cell *params);
	static NATIVE SetPlayerWeather(AMX *amx,const cell *params);
	static NATIVE SetPlayerColor(AMX *amx,const cell *params);
	static NATIVE GetPlayerColor(AMX *amx,const cell *params);
	static NATIVE SetPlayerName(AMX *amx,const cell *params);
	static NATIVE GetPlayerPing(AMX *amx,const cell *params);
	static NATIVE SetPlayerClothes(AMX *amx,const cell *params);
	static NATIVE GetPlayerClothes(AMX *amx,const cell *params);
	static NATIVE ResetPlayerClothes(AMX *amx,const cell *params);



};