/*
    IV:MP IRC Module v1.0
    Copyright © 2010 MaVe

    Original code from Incognito's SA:MP IRC Plugin

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../main.h"
#include "CCore.h"
#include "CIRC.h"
#include "CNatives.h"

namespace Natives
{

int IRC_Connect(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 7)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	const char *server;
	sq_getstring(pVM, -pcnt, &server);
	int port = 0;
	sq_getinteger(pVM, -pcnt+1, &port);
	if (port < 1 || port > 65535)
		port = 6667;
	const char *nickName;
	sq_getstring(pVM, -pcnt+2, &nickName);
	if (nickName == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *realName; sq_getstring(pVM, -pcnt+3, &realName);
	if (realName == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *userName; sq_getstring(pVM, -pcnt+4, &userName);
	if (userName == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	bool ssl = false;
	sq_getinteger(pVM, -pcnt+5, (int*)&ssl);
	const char *localIP; sq_getstring(pVM, -pcnt+6, &localIP);
	int	botID = 1;
	std::vector<int> sortedIDs;
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		sortedIDs.push_back((* b).main.botID);
	}
	std::sort(sortedIDs.begin(), sortedIDs.end());
	for (std::vector<int>::iterator s = sortedIDs.begin(); s != sortedIDs.end(); ++s)
	{
		if (*s == botID)
		{
			botID++;
		}
	}
	CIRC newBot;
	newBot.main.botID = botID;
	newBot.main.localIP = (localIP != NULL ? localIP : "");
	newBot.main.nickName = nickName;
	newBot.main.port = port;
	newBot.main.realName = realName;
	newBot.main.server = server;
	newBot.main.ssl = ssl;
	newBot.main.userName = userName;
	gIRC.push_back(newBot);
	//boost::thread createThread(IRC::create, boost::ref(Data::Core::bots.back()));
	gIRC.back().createThread();
	sq_pushinteger(pVM, botID);
	return 1;
}

/*
cell AMX_NATIVE_CALL
	Natives::IRC_Connect(AMX * amx, cell * params)
{
	CHECK_PARAMS(7, "IRC_Connect");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	char
		* server;
	amx_StrParam(amx, params[1], server);
	int
		port = params[2];
	if (port < 1 || port > 65535)
	{
		return 0;
	}
	char
		* nickName;
	amx_StrParam(amx, params[3], nickName);
	if (nickName == NULL)
	{
		return 0;
	}
	char
		* realName;
	amx_StrParam(amx, params[4], realName);
	if (realName == NULL)
	{
		return 0;
	}
	char
		* userName;
	amx_StrParam(amx, params[5], userName);
	if (userName == NULL)
	{
		return 0;
	}
	bool
		ssl = (params[6] != 0);
	char
		* localIP;
	amx_StrParam(amx, params[7], localIP);
	int
		botID = 1;
	std::vector<int>
		sortedIDs;
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		sortedIDs.push_back((* b).botID);
	}
	std::sort(sortedIDs.begin(), sortedIDs.end());
	for (std::vector<int>::iterator s = sortedIDs.begin(); s != sortedIDs.end(); ++s)
	{
		if (* s == botID)
		{
			botID++;
		}
	}
	Data::Bot
		newBot;
	newBot.botID = botID;
	newBot.localIP = (localIP ? localIP : "");
	newBot.nickName = nickName;
	newBot.port = port;
	newBot.realName = realName;
	newBot.server = server;
	newBot.ssl = ssl;
	newBot.userName = userName;
	Data::Core::bots.push_back(newBot);
	boost::thread
		createThread(IRC::create, boost::ref(Data::Core::bots.back()));
	return botID;
}*/

int IRC_Quit(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 2)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *message; sq_getstring(pVM, -pcnt+1, &message);
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((*b).main.botID == botID)
		{
			if ((*b).main.connected)
			{
				/*std::string sendBuffer = boost::str(boost::format("QUIT :%1%\r\n") % (message ? message : ""));
				IRC::transmit(*b, sendBuffer);
				shutdown((*b).clientSocket, 2);
				closesocket((*b).clientSocket);
				if ((*b).ssl)
				{
					SSL_shutdown((* b).sslSocket);
					SSL_CTX_free((* b).sslContext);
					SSL_free((* b).sslSocket);
				}*/
				std::string
					sendBuffer = boost::str(boost::format("QUIT :%1%\r\n") % (message ? message : ""));
				(* b).transmitMessage(sendBuffer);
				(* b).eraseBot();
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_Quit(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "IRC_Quit");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* message;
	amx_StrParam(amx, params[2], message);
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((* b).botID == botID)
		{
			if ((* b).connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("QUIT :%1%\r\n") % (message ? message : ""));
				IRC::transmit(* b, sendBuffer);
				shutdown((* b).clientSocket, 2);
				closesocket((* b).clientSocket);
				if ((* b).ssl)
				{
					SSL_shutdown((* b).sslSocket);
					SSL_CTX_free((* b).sslContext);
					SSL_free((* b).sslSocket);
				}
				return 1;
			}
		}
	}
	return 0;
}*/

int IRC_JoinChannel(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 2)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock mainLock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *channel; sq_getstring(pVM, -pcnt+1, &channel);
	if (channel == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *key = NULL;
	if (pcnt >= 3)
		sq_getstring(pVM, -pcnt+2, &key);
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		if ((* i).main.botID == botID)
		{
			if ((* i).main.connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("JOIN %1% %2%\r\n") % channel % (key ? key : ""));
				(* i).transmitMessage(sendBuffer);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_JoinChannel(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "IRC_JoinChannel");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* channel;
	amx_StrParam(amx, params[2], channel);
	if (channel == NULL)
	{
		return 0;
	}
	char
		* key;
	amx_StrParam(amx, params[3], key);
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((* b).botID == botID)
		{
			if ((* b).connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("JOIN %1% %2%\r\n") % channel % (key ? key : ""));
				IRC::transmit(* b, sendBuffer);
				return 1;
			}
		}
	}
	return 0;
}*/

int IRC_PartChannel(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 2)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *channel; sq_getstring(pVM, -pcnt+1, &channel);
	if (channel == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *message = NULL;
	if (pcnt >= 3)
		sq_getstring(pVM, -pcnt+2, &message);
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		if ((* i).main.botID == botID)
		{
			if ((* i).main.connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("PART %1% :%2%\r\n") % channel % (message ? message : ""));
				(* i).transmitMessage(sendBuffer);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_PartChannel(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "IRC_PartChannel");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* channel;
	amx_StrParam(amx, params[2], channel);
	if (channel == NULL)
	{
		return 0;
	}
	char
		* message;
	amx_StrParam(amx, params[3], message);
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((* b).botID == botID)
		{
			if ((* b).connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("PART %1% :%2%\r\n") % channel % (message ? message : ""));
				IRC::transmit(* b, sendBuffer);
				return 1;
			}
		}
	}
	return 0;
}*/

int IRC_ChangeNick(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 2)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *nickName; sq_getstring(pVM, -pcnt+1, &nickName);
	if (nickName == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		if ((* i).main.botID == botID)
		{
			if ((* i).main.connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("NICK %1%\r\n") % nickName);
				(* i).transmitMessage(sendBuffer);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_ChangeNick(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "IRC_ChangeNick");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* nickName;
	amx_StrParam(amx, params[2], nickName);
	if (nickName == NULL)
	{
		return 0;
	}
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((* b).botID == botID)
		{
			if ((* b).connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("NICK %1%\r\n") % nickName);
				IRC::transmit(* b, sendBuffer);
				return 1;
			}
		}
	}
	return 0;
}*/

int IRC_SetMode(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 3)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *target; sq_getstring(pVM, -pcnt+1, &target);
	if (target == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *mode; sq_getstring(pVM, -pcnt+2, &mode);
	if (mode == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		if ((* i).main.botID == botID)
		{
			if ((* i).main.connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("MODE %1% %2%\r\n") % target % mode);
				(* i).transmitMessage(sendBuffer);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_SetMode(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "IRC_SetMode");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* target;
	amx_StrParam(amx, params[2], target);
	if (target == NULL)
	{
		return 0;
	}
	char
		* mode;
	amx_StrParam(amx, params[3], mode);
	if (mode == NULL)
	{
		return 0;
	}
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((* b).botID == botID)
		{
			if ((* b).connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("MODE %1% %2%\r\n") % target % mode);
				IRC::transmit(* b, sendBuffer);
				return 1;
			}
		}
	}
	return 0;
}*/

int IRC_Say(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 3)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *target; sq_getstring(pVM, -pcnt+1, &target);
	if (target == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *message; sq_getstring(pVM, -pcnt+2, &message);
	if (message == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		if ((* i).main.botID == botID)
		{
			if ((* i).main.connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("PRIVMSG %1% :%2%\r\n") % target % message);
				(* i).transmitMessage(sendBuffer);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_Say(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "IRC_Say");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* target;
	amx_StrParam(amx, params[2], target);
	if (target == NULL)
	{
		return 0;
	}
	char
		* message;
	amx_StrParam(amx, params[3], message);
	if (message == NULL)
	{
		return 0;
	}
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((* b).botID == botID)
		{
			if ((* b).connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("PRIVMSG %1% :%2%\r\n") % target % message);
				IRC::transmit(* b, sendBuffer);
				return 1;
			}
		}
	}
	return 0;
}*/

int IRC_Notice(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 3)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *target; sq_getstring(pVM, -pcnt+1, &target);
	if (target == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *message; sq_getstring(pVM, -pcnt+2, &message);
	if (message == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		if ((* i).main.botID == botID)
		{
			if ((* i).main.connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("NOTICE %1% :%2%\r\n") % target % message);
				(* i).transmitMessage(sendBuffer);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_Notice(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "IRC_Notice");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* target;
	amx_StrParam(amx, params[2], target);
	if (target == NULL)
	{
		return 0;
	}
	char
		* message;
	amx_StrParam(amx, params[3], message);
	if (message == NULL)
	{
		return 0;
	}
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((* b).botID == botID)
		{
			if ((* b).connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("NOTICE %1% :%2%\r\n") % target % message);
				IRC::transmit(* b, sendBuffer);
				return 1;
			}
		}
	}
	return 0;
}*/

int IRC_IsUserOnChannel(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 3)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *channel; sq_getstring(pVM, -pcnt+1, &channel);
	if (channel == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *user; sq_getstring(pVM, -pcnt+2, &user);
	if (user == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		for (std::list<Data::Channel>::iterator c = (* i).channels.begin(); c != (* i).channels.end(); ++c)
		{
			if (!(* c).channel.compare(channel))
			{
				for (std::vector<std::string>::iterator n = (* c).names.begin(); n != (* c).names.end(); ++n)
				{
					switch ((* n).at(0))
					{
						case '+':
						case '%':
						case '@':
						case '&':
						case '!':
						case '*':
						case '~':
						case '.':
						{
							if (boost::algorithm::iequals((* n).substr(1), user))
							{
								sq_pushbool(pVM, true);
								return 1;
							}
						}
						break;
						default:
						{
							if (boost::algorithm::iequals(* n, user))
							{
								sq_pushbool(pVM, true);
								return 1;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}


/*cell AMX_NATIVE_CALL
	Natives::IRC_IsUserOnChannel(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "IRC_IsUserOnChannel");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* channel;
	amx_StrParam(amx, params[2], channel);
	if (channel == NULL)
	{
		return 0;
	}
	char
		* user;
	amx_StrParam(amx, params[3], user);
	if (user == NULL)
	{
		return 0;
	}
	int
		result = 0;
	for (std::list<Data::Channel>::iterator c = Data::Core::channels.begin(); c != Data::Core::channels.end(); ++c)
	{
		if ((* c).botID == botID)
		{
			if (!(* c).channel.compare(channel))
			{
				for (std::vector<std::string>::iterator n = (* c).names.begin(); n != (* c).names.end(); ++n)
				{
					switch ((* n).at(0))
					{
						case '+': case '%': case '@': case '&': case '!': case '*': case '~': case '.':
						{
							if (boost::iequals((* n).substr(1), user))
							{
								result = 1;
							}
						}
						break;
						default:
						{
							if (boost::iequals(* n, user))
							{
								result = 1;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}
	return result;
}*/

int IRC_InviteUser(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 3)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *channel; sq_getstring(pVM, -pcnt+1, &channel);
	if (channel == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *user; sq_getstring(pVM, -pcnt+2, &user);
	if (user == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		if ((* i).main.botID == botID)
		{
			if ((* i).main.connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("INVITE %1% %2%\r\n") % user % channel);
				(* i).transmitMessage(sendBuffer);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_InviteUser(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "IRC_InviteUser");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* channel;
	amx_StrParam(amx, params[2], channel);
	if (channel == NULL)
	{
		return 0;
	}
	char
		* user;
	amx_StrParam(amx, params[3], user);
	if (user == NULL)
	{
		return 0;
	}
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((* b).botID == botID)
		{
			if ((* b).connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("INVITE %1% %2%\r\n") % user % channel);
				IRC::transmit(* b, sendBuffer);
				return 1;
			}
		}
	}
	return 0;
}*/

int IRC_KickUser(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 3)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *channel; sq_getstring(pVM, -pcnt+1, &channel);
	if (channel == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *user; sq_getstring(pVM, -pcnt+2, &user);
	if (user == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *message = NULL;
	if (pcnt >= 4)
		sq_getstring(pVM, -pcnt+3, &message);
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		if ((* i).main.botID == botID)
		{
			if ((* i).main.connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("KICK %1% %2% :%3%\r\n") % channel % user % (message ? message : ""));
				(* i).transmitMessage(sendBuffer);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_KickUser(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "IRC_KickUser");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* channel;
	amx_StrParam(amx, params[2], channel);
	if (channel == NULL)
	{
		return 0;
	}
	char
		* user;
	amx_StrParam(amx, params[3], user);
	if (user == NULL)
	{
		return 0;
	}
	char
		* message;
	amx_StrParam(amx, params[4], message);
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((* b).botID == botID)
		{
			if ((* b).connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("KICK %1% %2% :%3%\r\n") % channel % user % (message ? message : ""));
				IRC::transmit(* b, sendBuffer);
				return 1;
			}
		}
	}
	return 0;
}*/

int IRC_GetUserChannelMode(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 3)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *channel; sq_getstring(pVM, -pcnt+1, &channel);
	if (channel == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *user; sq_getstring(pVM, -pcnt+2, &user);
	if (user == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	std::string level;
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		for (std::list<Data::Channel>::iterator c = (* i).channels.begin(); c != (* i).channels.end(); ++c)
		{
			if (!(* c).channel.compare(channel))
			{
				for (std::vector<std::string>::iterator n = (* c).names.begin(); n != (* c).names.end(); ++n)
				{
					switch ((* n).at(0))
					{
						case '+':
						case '%':
						case '@':
						case '&':
						case '!':
						case '*':
						case '~':
						case '.':
						{
							if (boost::algorithm::iequals((* n).substr(1), user))
							{
								level = (* n).at(0);
							}
						}
						break;
						default:
						{
							if (boost::algorithm::iequals(* n, user))
							{
								level = "-";
							}
						}
						break;
					}
					if (!level.empty())
					{
						break;
					}
				}
				break;
			}
		}
	}
	if (level.empty())
	{
		level = "-";
	}
	sq_pushstring(pVM, level.c_str(), -1);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_GetUserChannelMode(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "IRC_GetUserChannelMode");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* channel;
	amx_StrParam(amx, params[2], channel);
	if (channel == NULL)
	{
		return 0;
	}
	char
		* user;
	amx_StrParam(amx, params[3], user);
	if (user == NULL)
	{
		return 0;
	}
	cell
		* destination;
	amx_GetAddr(amx, params[4], &destination);
	if (destination == NULL)
	{
		return 0;
	}
	std::string
		level;
	for (std::list<Data::Channel>::iterator c = Data::Core::channels.begin(); c != Data::Core::channels.end(); ++c)
	{
		if ((* c).botID == botID)
		{
			if (!(* c).channel.compare(channel))
			{
				for (std::vector<std::string>::iterator n = (* c).names.begin(); n != (* c).names.end(); ++n)
				{
					switch ((* n).at(0))
					{
						case '+': case '%': case '@': case '&': case '!': case '*': case '~': case '.':
						{
							if (boost::iequals((* n).substr(1), user))
							{
								level = (* n).at(0);
							}
						}
						break;
						default:
						{
							if (boost::iequals(* n, user))
							{
								level = "-";
							}
						}
						break;
					}
					if (!level.empty())
					{
						break;
					}
				}
				break;
			}
		}
	}
	if (level.empty())
	{
		level = "-";
	}
	amx_SetString(destination, level.c_str(), 0, 0, level.length() + 1);
	return 1;
}*/

int IRC_GetChannelUserList(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 2)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *channel; sq_getstring(pVM, -pcnt+1, &channel);
	if (channel == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	std::string userList;
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		for (std::list<Data::Channel>::iterator c = (* i).channels.begin(); c != (* i).channels.end(); ++c)
		{
			for (std::list<Data::Channel>::iterator c = (* i).channels.begin(); c != (* i).channels.end(); ++c)
			{
				if (!(* c).channel.compare(channel))
				{
					for (std::vector<std::string>::iterator n = (* c).names.begin(); n != (* c).names.end(); ++n)
					{
						userList += * n + " ";
					}
					break;
				}
			}
		}
	}
	if (userList.empty())
	{
		userList = "None";
	}
	else
	{
		userList.resize(userList.length() - 1);
	}
	sq_pushstring(pVM, userList.c_str(), -1);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_GetChannelUserList(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "IRC_GetChannelUserList");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* channel;
	amx_StrParam(amx, params[2], channel);
	if (channel == NULL)
	{
		return 0;
	}
	cell
		* destination;
	amx_GetAddr(amx, params[3], &destination);
	if (destination == NULL)
	{
		return 0;
	}
	int
		maxLength = params[4];
	if (!maxLength)
	{
		return 0;
	}
	std::string
		userList;
	for (std::list<Data::Channel>::iterator c = Data::Core::channels.begin(); c != Data::Core::channels.end(); ++c)
	{
		if ((* c).botID == botID)
		{
			if (!(* c).channel.compare(channel))
			{
				for (std::vector<std::string>::iterator n = (* c).names.begin(); n != (* c).names.end(); ++n)
				{
					userList += * n + " ";
				}
				break;
			}
		}
	}
	if (userList.empty())
	{
		userList = "None";
	}
	else
	{
		userList.resize(userList.length() - 1);
	}
	amx_SetString(destination, userList.c_str(), 0, 0, maxLength);
	return 1;
}*/

int IRC_SetChannelTopic(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 3)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *channel; sq_getstring(pVM, -pcnt+1, &channel);
	if (channel == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *topic; sq_getstring(pVM, -pcnt+2, &topic);
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		if ((* i).main.botID == botID)
		{
			if ((* i).main.connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("TOPIC %1% :%2%\r\n") % channel % (topic ? topic : ""));
				(* i).transmitMessage(sendBuffer);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_SetChannelTopic(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "IRC_SetChannelTopic");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* channel;
	amx_StrParam(amx, params[2], channel);
	if (channel == NULL)
	{
		return 0;
	}
	char
		* topic;
	amx_StrParam(amx, params[3], topic);
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((* b).botID == botID)
		{
			if ((* b).connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("TOPIC %1% :%2%\r\n") % channel % (topic ? topic : ""));
				IRC::transmit(* b, sendBuffer);
				return 1;
			}
		}
	}
	return 0;
}*/

int IRC_SendRaw(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 2)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int botID = 0; sq_getinteger(pVM, -pcnt, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *message; sq_getstring(pVM, -pcnt+1, &message);
	if (message == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		if ((* i).main.botID == botID)
		{
			if ((* i).main.connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("%1%\r\n") % message);
				(* i).transmitMessage(sendBuffer);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_SendRaw(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "IRC_SendRaw");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		botID = params[1];
	if (!botID)
	{
		return 0;
	}
	char
		* message;
	amx_StrParam(amx, params[2], message);
	if (message == NULL)
	{
		return 0;
	}
	for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
	{
		if ((* b).botID == botID)
		{
			if ((* b).connected)
			{
				std::string
					sendBuffer = boost::str(boost::format("%1%\r\n") % message);
				IRC::transmit(* b, sendBuffer);
				return 1;
			}
		}
	}
	return 0;
}*/

int IRC_CreateGroup(HSQUIRRELVM pVM)
{
	boost::mutex::scoped_lock lock(CCore::mutex);
	int groupID = 1;
	std::vector<int> sortedIDs;
	for (std::vector<Data::Group>::iterator g = CCore::groups.begin(); g != CCore::groups.end(); ++g)
	{
		sortedIDs.push_back((* g).groupID);
	}
	std::sort(sortedIDs.begin(), sortedIDs.end());
	for (std::vector<int>::iterator s = sortedIDs.begin(); s != sortedIDs.end(); ++s)
	{
		if (* s == groupID)
		{
			groupID++;
		}
	}
	Data::Group newGroup;
	newGroup.groupID = groupID;
	CCore::groups.push_back(newGroup);
	sq_pushinteger(pVM, groupID);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_CreateGroup(AMX * amx, cell * params)
{
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		groupID = 1;
	std::vector<int>
		sortedIDs;
	for (std::vector<Data::Group>::iterator g = Data::Core::groups.begin(); g != Data::Core::groups.end(); ++g)
	{
		sortedIDs.push_back((* g).groupID);
	}
	std::sort(sortedIDs.begin(), sortedIDs.end());
	for (std::vector<int>::iterator s = sortedIDs.begin(); s != sortedIDs.end(); ++s)
	{
		if (* s == groupID)
		{
			groupID++;
		}
	}
	Data::Group
		newGroup;
	newGroup.groupID = groupID;
	Data::Core::groups.push_back(newGroup);
	return groupID;
}*/

int IRC_DestroyGroup(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 1)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int groupID = 0; sq_getinteger(pVM, -pcnt, &groupID);
	if (!groupID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	for (std::vector<Data::Group>::iterator g = CCore::groups.begin(); g != CCore::groups.end(); ++g)
	{
		if ((* g).groupID == groupID)
		{
			CCore::groups.erase(g);
			sq_pushbool(pVM, true);
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_DestroyGroup(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "IRC_DestroyGroup");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		groupID = params[1];
	if (!groupID)
	{
		return 0;
	}
	for (std::vector<Data::Group>::iterator g = Data::Core::groups.begin(); g != Data::Core::groups.end(); ++g)
	{
		if ((* g).groupID == groupID)
		{
			Data::Core::groups.erase(g);
			return 1;
		}
	}
	return 0;
}*/

int IRC_AddToGroup(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 2)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int groupID = 0; sq_getinteger(pVM, -pcnt, &groupID);
	if (!groupID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	int botID = 0; sq_getinteger(pVM, -pcnt+1, &botID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	for (std::vector<Data::Group>::iterator g = CCore::groups.begin(); g != CCore::groups.end();++g)
	{
		if ((* g).groupID == groupID)
		{
			for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
			{
				if ((* b).first == botID)
				{
					sq_pushbool(pVM, false);
					return 1;
				}
			}
			for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
			{
				if ((* i).main.botID == botID)
				{
					(* i).main.groupID = groupID;
					break;
				}
			}
			(* g).bots.insert(std::pair<int, bool>(botID, false));
			sq_pushbool(pVM, true);
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_AddToGroup(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "IRC_AddToGroup");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		groupID = params[1];
	if (!groupID)
	{
		return 0;
	}
	int
		botID = params[2];
	if (!botID)
	{
		return 0;
	}
	for (std::vector<Data::Group>::iterator g = Data::Core::groups.begin(); g != Data::Core::groups.end();++g)
	{
		if ((* g).groupID == groupID)
		{
			for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
			{
				if ((* b).first == botID)
				{
					return 0;
				}
			}
			(* g).bots.insert(std::pair<int, bool>(botID, false));
			return 1;
		}
	}
	return 0;
}*/

int IRC_RemoveFromGroup(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 2)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int groupID = 0; sq_getinteger(pVM, -pcnt, &groupID);
	if (!groupID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	int botID = 0; sq_getinteger(pVM, -pcnt+1, &groupID);
	if (!botID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	for (std::vector<Data::Group>::iterator g = CCore::groups.begin(); g != CCore::groups.end(); ++g)
	{
		if ((* g).groupID == groupID)
		{
			for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
			{
				if ((* b).first == botID)
				{
					for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
					{
						if ((* i).main.botID == botID)
						{
							(* i).main.groupID = 0;
							break;
						}
					}
					(* g).bots.erase(b);
					sq_pushbool(pVM, true);
					return 1;
				}
			}
			break;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_RemoveFromGroup(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "IRC_RemoveFromGroup");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		groupID = params[1];
	if (!groupID)
	{
		return 0;
	}
	int
		botID = params[2];
	if (!botID)
	{
		return 0;
	}
	for (std::vector<Data::Group>::iterator g = Data::Core::groups.begin(); g != Data::Core::groups.end(); ++g)
	{
		if ((* g).groupID == groupID)
		{
			for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
			{
				if ((* b).first == botID)
				{
					(* g).bots.erase(b);
					return 1;
				}
			}
			break;
		}
	}
	return 0;
}*/

int IRC_GroupSay(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 3)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	boost::mutex::scoped_lock lock(CCore::mutex);
	int groupID = 0; sq_getinteger(pVM, -pcnt, &groupID);
	if (!groupID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *target; sq_getstring(pVM, -pcnt+1, &target);
	if (target == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *message; sq_getstring(pVM, -pcnt+2, &message);
	if (message == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	int botID = 0;
	for (std::vector<Data::Group>::iterator g = CCore::groups.begin(); g != CCore::groups.end(); ++g)
	{
		if ((* g).groupID == groupID)
		{
			if ((* g).bots.empty())
			{
				break;
			}
			for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
			{
				if (!(* b).second)
				{
					botID = (* b).first;
					(* b).second = true;
					break;
				}
			}
			if (!botID)
			{
				for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
				{
					(* b).second = false;
				}
				std::map<int, bool>::iterator
					s = (* g).bots.begin();
				botID = (* s).first;
				(* s).second = true;
			}
			break;
		}
	}
	if (botID)
	{
		for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
		{
			if ((* i).main.botID == botID)
			{
				if ((* i).main.connected)
				{
					std::string
						sendBuffer = boost::str(boost::format("PRIVMSG %1% :%2%\r\n") % target % message);
					(* i).transmitMessage(sendBuffer);
					sq_pushbool(pVM, true);
					return 1;
				}
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_GroupSay(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "IRC_GroupSay");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		groupID = params[1];
	if (!groupID)
	{
		return 0;
	}
	char
		* target;
	amx_StrParam(amx, params[2], target);
	if (target == NULL)
	{
		return 0;
	}
	char
		* message;
	amx_StrParam(amx, params[3], message);
	if (message == NULL)
	{
		return 0;
	}
	int
		botID = 0;
	for (std::vector<Data::Group>::iterator g = Data::Core::groups.begin(); g != Data::Core::groups.end(); ++g)
	{
		if ((* g).groupID == groupID)
		{
			if ((* g).bots.empty())
			{
				break;
			}
			for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
			{
				if (!(* b).second)
				{
					botID = (* b).first;
					(* b).second = true;
					break;
				}
			}
			if (!botID)
			{
				for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
				{
					(* b).second = false;
				}
				std::map<int, bool>::iterator
					s = (* g).bots.begin();
				botID = (* s).first;
				(* s).second = true;
			}
			break;
		}
	}
	if (botID)
	{
		for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
		{
			if ((* b).botID == botID)
			{
				if ((* b).connected)
				{
					std::string
						sendBuffer = boost::str(boost::format("PRIVMSG %1% :%2%\r\n") % target % message);
					IRC::transmit(* b, sendBuffer);
					return 1;
				}
			}
		}
	}
	return 0;
}*/

int IRC_GroupNotice(HSQUIRRELVM pVM)
{
	int pcnt = sq_gettop(pVM) - 1;
	if (pcnt < 3)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	boost::mutex::scoped_lock lock(CCore::mutex);
	int groupID = 0; sq_getinteger(pVM, -pcnt, &groupID);
	if (!groupID)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *target; sq_getstring(pVM, -pcnt+1, &target);
	if (target == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	const char *message; sq_getstring(pVM, -pcnt+2, &message);
	if (message == NULL)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	int botID = 0;
	for (std::vector<Data::Group>::iterator g = CCore::groups.begin(); g != CCore::groups.end(); ++g)
	{
		if ((* g).groupID == groupID)
		{
			if ((* g).bots.empty())
			{
				break;
			}
			for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
			{
				if (!(* b).second)
				{
					botID = (* b).first;
					(* b).second = true;
					break;
				}
			}
			if (!botID)
			{
				for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
				{
					(* b).second = false;
				}
				std::map<int, bool>::iterator
					s = (* g).bots.begin();
				botID = (* s).first;
				(* s).second = true;
			}
			break;
		}
	}
	if (botID)
	{
		for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
		{
			if ((* i).main.botID == botID)
			{
				if ((* i).main.connected)
				{
					std::string
						sendBuffer = boost::str(boost::format("NOTICE %1% :%2%\r\n") % target % message);
					(* i).transmitMessage(sendBuffer);
					sq_pushbool(pVM, true);
					return 1;
				}
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

/*cell AMX_NATIVE_CALL
	Natives::IRC_GroupNotice(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "IRC_GroupNotice");
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	int
		groupID = params[1];
	if (!groupID)
	{
		return 0;
	}
	char
		* target;
	amx_StrParam(amx, params[2], target);
	if (target == NULL)
	{
		return 0;
	}
	char
		* message;
	amx_StrParam(amx, params[3], message);
	if (message == NULL)
	{
		return 0;
	}
	int
		botID = 0;
	for (std::vector<Data::Group>::iterator g = Data::Core::groups.begin(); g != Data::Core::groups.end(); ++g)
	{
		if ((* g).groupID == groupID)
		{
			if ((* g).bots.empty())
			{
				break;
			}
			for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
			{
				if (!(* b).second)
				{
					botID = (* b).first;
					(* b).second = true;
					break;
				}
			}
			if (!botID)
			{
				for (std::map<int, bool>::iterator b = (* g).bots.begin(); b != (* g).bots.end(); ++b)
				{
					(* b).second = false;
				}
				std::map<int, bool>::iterator
					s = (* g).bots.begin();
				botID = (* s).first;
				(* s).second = true;
			}
			break;
		}
	}
	if (botID)
	{
		for (std::list<CIRC>::iterator b = gIRC.begin(); b != gIRC.end(); ++b)
		{
			if ((* b).botID == botID)
			{
				if ((* b).connected)
				{
					std::string
						sendBuffer = boost::str(boost::format("NOTICE %1% :%2%\r\n") % target % message);
					IRC::transmit(* b, sendBuffer);
					return 1;
				}
			}
		}
	}
	return 0;
}
*/
}