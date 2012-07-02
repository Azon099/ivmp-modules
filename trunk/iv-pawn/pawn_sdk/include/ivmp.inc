#include <core>
#include <string>
#include <time>
#include <float>
#include <rational>
#include <console>
#include <args>

// Defines
#define MAX_VEHICLES 0xFFFE
#define MAX_OBJECTS 0xFFFE
#define MAX_CHECKPOINTS 0xFFFE
#define MAX_BLIPS 1300
#define MAX_ACTORS 100
#define MAX_PICKUPS 0xFFFE
#define MAX_NAME 32

// Enums

enum eStateType
{
	STATE_TYPE_DISCONNECT,
	STATE_TYPE_CONNECT,
	STATE_TYPE_SPAWN,
	STATE_TYPE_DEATH,
	STATE_TYPE_ONFOOT,
	STATE_TYPE_ENTERVEHICLE,
	STATE_TYPE_INVEHICLE,
	STATE_TYPE_PASSENGER,
	STATE_TYPE_EXITVEHICLE
};

// Callbacks

forward OnInit();
forward OnPlayerConnect(playerid);
forward OnPlayerDisconnect(playerid);
forward OnPlayerNameCheck(playerid, name[]);
forward OnPlayerText(playerid, text);
forward OnPlayerCommand(playerid, cmd[]);
forward OnPlayerSpawn(playerid);
forward OnPlayerDeath(playerid);
forward OnPlayerEnterVehicle(playerid, vehicleid, seatid);
forward OnPlayerCancelVehicleEntry(playerid);
forward OnPlayerChangeState(playerid, oldstate, newstate);
forward OnPlayerChangePadState(playerid);
forward OnVehicleCreate(vehicleid);
forward OnVehicleDelete(vehicleid);
forward OnObjectCreate(objectid);
forward OnObjectDelete(objectid);
forward OnPlayerEnterCheckpoint(playerid, cpid);
forward OnPlayerLeaveCheckpoint(playerid, cpid);
forward OnBlipCreate(blipid);
forward OnBlipDelete(blipid);
forward OnActorCreate(actorid);
forward OnActorDelete(actorid);

// Functions
native IsPlayerConnected(playerid);
native SetPlayerHealth(playerid, health);
native GetPlayerHealth(playerid);
native SetPlayerCoordinates(playerid, Float:X, Float:Y, Float:Z);
native GetPlayerCoordinates(playerid, &Float:X, &Float:Y, &Float:Z);
native SetPlayerHeading(playerid, heading);
native GetPlayerHeading(playerid);
native SetPlayerModel(playerid, modelid);
native GetPlayerModel(playerid);
// native SetPlayerTime();
// native SetPlayerGravity();
// native SendPlayerMessage();
native GetPlayerVehicleId(playerid);
// native IsPlayerOnFoot();
// native IsPlayerInVehicle();
native IsPlayerSpawned(playerid);
native SetPlayerArmour(playerid, armour);
native GetPlayerArmour(playerid);
native SetPlayerMoney(playerid, money);
native GetPlayerMoney(playerid);
native GivePlayerMoney(playerid, money);
native ResetPlayerMoney(playerid);
native SetPlayerVelocity(playerid, Float:X, Float:Y, Float:Z);
native GetPlayerVelocity(playerid, &Float:X, &Float:Y, &Float:Z);
native GetPlayerState(playerid);
// native SetPlayerWantedLevel();
native IsPlayerInAnyVehicle(playerid);
// native WarpPlayerIntoVehicle();
// native RemovePlayerFromVehicle();
// native GivePlayerWeapon();
// native RemovePlayerWeapons();
native GetPlayerAmmo(playerid);
native GetPlayerWeapon(playerid);
native GetPlayerSeatId(playerid);
// native SetPlayerInterior();
// native GetPlayerInterior();
// native SetPlayerWeather();
native SetPlayerColor(playerid, color);
native GetPlayerColor(playerid);
// native SetPlayerName();
native GetPlayerPing();
native SetPlayerClothes(playerid, clothside, clothid);
native GetPlayerClothes(playerid, clothside);
native ResetPlayerClothes(playerid);

