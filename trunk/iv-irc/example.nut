/*

IV:MP IRC Module v1.0

Basic example script (Beta 1)

*/

local iBot;

local strEchoChannel = "#ivmp";

function onScriptInit()
{
	ircRegisterMe();
	
	iBot = ircConnect("irc.gtanet.com", 6667, "IVMPirc", "IVMP", "IVMP", false, "");
	
	return true;
}
addEvent("scriptInit", onScriptInit);

function ircOnConnect(iBotid)
{
	print("connect");
	ircJoinChannel(iBotid, strEchoChannel);
	
	return true;
}

function ircOnReceiveRaw(botid, data)
{
	return true;
}

function onPlayerConnect(playerid)
{
	ircSay(iBot, strEchoChannel, "* " + getPlayerName(playerid) + " has joined the server.");
	
	return true;
}
addEvent("playerConnect", onPlayerConnect);

function onPlayerDisconnect(playerid, reason)
{
	ircSay(iBot, strEchoChannel, "* " + getPlayerName(playerid) + " has left the server.");
	
	return true;
}
addEvent("playerDisconnect", onPlayerDisconnect);

function onPlayerText(playerid, text)
{
	ircSay(iBot, strEchoChannel, getPlayerName(playerid) + ": " + text);
	
	return true;
}
addEvent("playerText", onPlayerText);

function ircOnUserSay(botid, receiver, user, host, message)
{
	sendMessageToAll("[irc] <" + user + "> " + message, 0xFFFFFFAA);
	
	return true;
}
