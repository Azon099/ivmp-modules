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

#include "main.h"
#include "Source/CCore.h"
#include "Source/CIRC.h"
#include "Source/CNatives.h"

std::list<HSQUIRRELVM> lstScripts;

int RegisterMe(HSQUIRRELVM pVM)
{
	lstScripts.push_back(pVM);

	sq_pushbool(pVM, true);
	return 1;
}

EXPORT bool InitModule(char *szModuleName)
{
	LogPrintf("[%s] Our module got loaded!", szModuleName);
	strcpy(szModuleName, "IRC Module");

	return true;
}

EXPORT void ScriptLoad(HSQUIRRELVM pVM)
{
	RegisterFunction(pVM, "ircRegisterMe", RegisterMe);
	RegisterFunction(pVM, "ircConnect", Natives::IRC_Connect);
	RegisterFunction(pVM, "ircQuit", Natives::IRC_Quit);
	RegisterFunction(pVM, "ircJoinChannel", Natives::IRC_JoinChannel);
	RegisterFunction(pVM, "ircPartChannel", Natives::IRC_PartChannel);
	RegisterFunction(pVM, "ircChangeNick", Natives::IRC_ChangeNick);
	RegisterFunction(pVM, "ircSetMode", Natives::IRC_SetMode);
	RegisterFunction(pVM, "ircSay", Natives::IRC_Say);
	RegisterFunction(pVM, "ircNotice", Natives::IRC_Notice);
	RegisterFunction(pVM, "ircIsUserOnChannel", Natives::IRC_IsUserOnChannel);
	RegisterFunction(pVM, "ircInviteUser", Natives::IRC_InviteUser);
	RegisterFunction(pVM, "ircKickUser", Natives::IRC_KickUser);
	RegisterFunction(pVM, "ircGetUserChannelMode", Natives::IRC_GetUserChannelMode);
	RegisterFunction(pVM, "ircGetChannelUserList", Natives::IRC_GetChannelUserList);
	RegisterFunction(pVM, "ircSetChannelTopic", Natives::IRC_SetChannelTopic);
	RegisterFunction(pVM, "ircSendRaw", Natives::IRC_SendRaw);
	RegisterFunction(pVM, "ircCreateGroup", Natives::IRC_CreateGroup);
	RegisterFunction(pVM, "ircDestroyGroup", Natives::IRC_DestroyGroup);
	RegisterFunction(pVM, "ircAddToGroup", Natives::IRC_AddToGroup);
	RegisterFunction(pVM, "ircRemoveFromGroup", Natives::IRC_RemoveFromGroup);
	RegisterFunction(pVM, "ircGroupSay", Natives::IRC_GroupSay);
	RegisterFunction(pVM, "ircGroupNotice", Natives::IRC_GroupNotice);
}

EXPORT void ScriptUnload(HSQUIRRELVM pVM)
{
	lstScripts.remove(pVM);
}

int CallFunction(const char *funcname, int defaultretval, const char *format, ...)
{
	for (std::list<HSQUIRRELVM>::iterator i = lstScripts.begin(); i != lstScripts.end(); ++i)
	{
		HSQUIRRELVM pVM = *i;

		int top = sq_gettop(pVM); // saves the stack size before the call
		sq_pushroottable(pVM); // pushes the global table
		sq_pushstring(pVM, funcname, -1);
		if(SQ_SUCCEEDED(sq_get(pVM, -2))) 
		{
			sq_pushroottable(pVM); // push the 'this' (in this case is the global table)
			int params = 1;
			const char* p = format;
			if(p != NULL)
			{
				va_list ap;
				va_start(ap, format);
				while(*p)
				{
					switch(*p)
					{
					case 'b':
						{
							bool b = va_arg(ap, int) != 0;
							sq_pushbool(pVM, b);
							params++;
							break;
						}
					case 'i':
						{
							int i = va_arg(ap, int);
							sq_pushinteger(pVM, i);
							params++;
							break;
						}
					case 'f':
						{
							float f = (float)va_arg(ap, double);
							sq_pushfloat(pVM, f);
							params++;
							break;
						}
					case 's':
						{
							char* s = va_arg(ap, char*);
							sq_pushstring(pVM, s, -1);
							params++;
							break;
						}
					}
					++p;
				}
				va_end(ap);
			}
			sq_call(pVM, params, true, true);
			int retval;
			sq_getinteger(pVM, -1, &retval);
			if (retval != defaultretval)
				return retval;
		}
		sq_settop(pVM, top); // restores the original stack size
	}
	return defaultretval;
}

EXPORT void Pulse()
{
	boost::mutex::scoped_lock lock(CCore::mutex);
	if (!CCore::queue.empty())
	{
		Data::Queue copiedQueue(CCore::queue.front());
		CCore::queue.pop();
		lock.unlock();

		switch (copiedQueue.type)
		{
			case 0:
			{
				CallFunction("ircOnConnect", 1, "i", copiedQueue.id);
			}
			break;
			case 1:
			{
				CallFunction("ircOnDisconnect", 1, "i", copiedQueue.id);
			}
			break;
			case 2:
			{
				CallFunction("ircOnJoinChannel", 1, "is", copiedQueue.id, copiedQueue.buffer.at(0).c_str());
			}
			break;
			case 3:
			{
				CallFunction("ircOnDisconnect", 1, "iss", copiedQueue.id, copiedQueue.buffer.at(1).c_str(), copiedQueue.buffer.at(0).c_str());
			}
			break;
			case 4:
			{
				CallFunction("ircOnUserDisconnect", 1, "isss", copiedQueue.id, copiedQueue.buffer.at(2).c_str(), copiedQueue.buffer.at(1).c_str(), copiedQueue.buffer.at(0).c_str());
			}
			break;
			case 5:
			{
				CallFunction("ircOnUserJoinChannel", 1, "isss", copiedQueue.id, copiedQueue.buffer.at(2).c_str(), copiedQueue.buffer.at(1).c_str(), copiedQueue.buffer.at(0).c_str());
			}
			break;
			case 6:
			{
				CallFunction("ircOnUserLeaveChannel", 1, "issss", copiedQueue.id, copiedQueue.buffer.at(3).c_str(), copiedQueue.buffer.at(2).c_str(), copiedQueue.buffer.at(1).c_str(), copiedQueue.buffer.at(0).c_str());
			}
			break;
			case 7:
			{
				CallFunction("ircOnUserNickChange", 1, "isss", copiedQueue.id, copiedQueue.buffer.at(2).c_str(), copiedQueue.buffer.at(1).c_str(), copiedQueue.buffer.at(0).c_str());
			}
			break;
			case 8:
			{
				CallFunction("ircOnUserSetChannelMode", 1, "issss", copiedQueue.id, copiedQueue.buffer.at(3).c_str(), copiedQueue.buffer.at(2).c_str(), copiedQueue.buffer.at(1).c_str(), copiedQueue.buffer.at(0).c_str());
			}
			break;
			case 9:
			{
				CallFunction("ircOnUserSetChannelTopic", 1, "issss", copiedQueue.id, copiedQueue.buffer.at(3).c_str(), copiedQueue.buffer.at(2).c_str(), copiedQueue.buffer.at(1).c_str(), copiedQueue.buffer.at(0).c_str());
			}
			break;
			case 10:
			{
				CallFunction("ircOnUserSay", 1, "issss", copiedQueue.id, copiedQueue.buffer.at(3).c_str(), copiedQueue.buffer.at(2).c_str(), copiedQueue.buffer.at(1).c_str(), copiedQueue.buffer.at(0).c_str());
			}
			break;
			case 11:
			{
				CallFunction("ircOnUserNotice", 1, "issss", copiedQueue.id, copiedQueue.buffer.at(3).c_str(), copiedQueue.buffer.at(2).c_str(), copiedQueue.buffer.at(1).c_str(), copiedQueue.buffer.at(0).c_str());
			}
			break;
			case 12:
			{
				CallFunction("ircOnReceiveRaw", 1, "is", copiedQueue.id, copiedQueue.buffer.at(0).c_str());
			}
			
			break;
		}
	}
}
