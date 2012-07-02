#include "CPawnNatives.h"

#pragma warning( disable : 4244 )

#define PLAYER_CHECK(x) \
		if(!GetPlayerManager()->DoesExist(x)) \
			return 0; \

#define VEHICLE_CHECK(x) \
		if(!GetVehicleManager()->DoesExist(x)) \
			return 0; \

#define GET_FLOATS(i, x) \
		cell *dest[i]; \
		for(int d = 0; d < i; d++) \
		{ \
			amx_GetAddr(amx, params[d+x], &dest[d]); \
		} \

#define SET_FLOAT(i, f) \
		*dest[i] = amx_ftoc(f); \

NATIVE CPawnNatives::IsPlayerConnected(AMX *amx,const cell *params)
{
	return GetPlayerManager()->DoesExist(params[0]);
}

NATIVE CPawnNatives::SetPlayerHealth(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->SetHealth(params[1]);
	return 1;
}

NATIVE CPawnNatives::GetPlayerHealth(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetHealth();
}

NATIVE CPawnNatives::SetPlayerCoordinates(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->SetPosition(Vector3(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])));
	return 1;
}

NATIVE CPawnNatives::GetPlayerCoordinates(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	Vector3 pos;
	GetPlayerManager()->GetAt(params[0])->GetPosition(&pos);
	GET_FLOATS(3, 1);
	SET_FLOAT(0, pos.X);
	SET_FLOAT(1, pos.Y);
	SET_FLOAT(2, pos.Z);
	return 1;
}

NATIVE CPawnNatives::SetPlayerHeading(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->SetHeading(amx_ctof(params[1]));
	return 1;
}

NATIVE CPawnNatives::GetPlayerHeading(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetHeading();
}

NATIVE CPawnNatives::SetPlayerModel(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->SetModel(params[1]);
	return 1;
}

NATIVE CPawnNatives::GetPlayerModel(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetModel();
}

NATIVE CPawnNatives::SetPlayerTime(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::SetPlayerGravity(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::SendPlayerMessage(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::GetPlayerVehicleId(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetVehicleId();
}

NATIVE CPawnNatives::IsPlayerOnFoot(AMX *amx,const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::IsPlayerInVehicle(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::IsPlayerSpawned(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->IsSpawned();
}

NATIVE CPawnNatives::SetPlayerArmour(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->SetArmour(params[1]);
	return 1;
}

NATIVE CPawnNatives::GetPlayerArmour(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetArmour();
}

NATIVE CPawnNatives::SetPlayerMoney(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->SetMoney(params[1]);
	return 1;
}

NATIVE CPawnNatives::GetPlayerMoney(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetMoney();
}

NATIVE CPawnNatives::GivePlayerMoney(AMX *amx,const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->GiveMoney(params[1]);
	return 1;
}

NATIVE CPawnNatives::ResetPlayerMoney(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->SetMoney(0);
	return 1;
}

NATIVE CPawnNatives::SetPlayerVelocity(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->SetMoveSpeed(Vector3(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])));
	return 1;
}

NATIVE CPawnNatives::GetPlayerVelocity(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GET_FLOATS(3, 1);
	Vector3 pos;
	GetPlayerManager()->GetAt(params[0])->GetMoveSpeed(&pos);
	SET_FLOAT(0, pos.X);
	SET_FLOAT(1, pos.Y);
	SET_FLOAT(2, pos.Z);
	return 1;
}

NATIVE CPawnNatives::GetPlayerState(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetState();
}

NATIVE CPawnNatives::SetPlayerWantedLevel(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::IsPlayerInAnyVehicle(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->IsInAnyVehicle();
}

NATIVE CPawnNatives::WarpPlayerIntoVehicle(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::RemovePlayerFromVehicle(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::GivePlayerWeapon(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	// GetPlayerManager()->GetAt(params[0])->GiveWeapon(params[1], params[2]);
	return 1;
}

NATIVE CPawnNatives::RemovePlayerWeapons(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::GetPlayerAmmo(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetAmmo();
}

NATIVE CPawnNatives::GetPlayerWeapon(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetWeapon();
}

NATIVE CPawnNatives::SetCameraBehindPlayer(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::GetPlayerSeatId(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetSeatId();
}

NATIVE CPawnNatives::SetPlayerInterior(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	// GetPlayerManager()->GetAt(params[0])->SetInterior(params[1]);
	return 1;
}

NATIVE CPawnNatives::GetPlayerInterior(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::SetPlayerWeather(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::SetPlayerColor(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->SetColor(params[1]);
	return 1;
}

NATIVE CPawnNatives::GetPlayerColor(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetColor();
}

NATIVE CPawnNatives::SetPlayerName(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return 1;
}

NATIVE CPawnNatives::GetPlayerPing(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetPing();
}

NATIVE CPawnNatives::SetPlayerClothes(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->SetClothes(params[1], params[2]);
	return 1;
}

NATIVE CPawnNatives::GetPlayerClothes(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	return GetPlayerManager()->GetAt(params[0])->GetClothes(params[1]);
}

NATIVE CPawnNatives::ResetPlayerClothes(AMX *amx, const cell *params)
{
	PLAYER_CHECK(params[0]);
	GetPlayerManager()->GetAt(params[0])->ResetClothes();
	return 1;
}