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

namespace Natives
{
	int IRC_Connect(HSQUIRRELVM pVM);
	int IRC_Quit(HSQUIRRELVM pVM);
	int IRC_JoinChannel(HSQUIRRELVM pVM);
	int IRC_PartChannel(HSQUIRRELVM pVM);
	int IRC_ChangeNick(HSQUIRRELVM pVM);
	int IRC_SetMode(HSQUIRRELVM pVM);
	int IRC_Say(HSQUIRRELVM pVM);
	int IRC_Notice(HSQUIRRELVM pVM);
	int IRC_IsUserOnChannel(HSQUIRRELVM pVM);
	int IRC_InviteUser(HSQUIRRELVM pVM);
	int IRC_KickUser(HSQUIRRELVM pVM);
	int IRC_GetUserChannelMode(HSQUIRRELVM pVM);
	int IRC_GetChannelUserList(HSQUIRRELVM pVM);
	int IRC_SetChannelTopic(HSQUIRRELVM pVM);
	int IRC_SendRaw(HSQUIRRELVM pVM);
	int IRC_CreateGroup(HSQUIRRELVM pVM);
	int IRC_DestroyGroup(HSQUIRRELVM pVM);
	int IRC_AddToGroup(HSQUIRRELVM pVM);
	int IRC_RemoveFromGroup(HSQUIRRELVM pVM);
	int IRC_GroupSay(HSQUIRRELVM pVM);
	int IRC_GroupNotice(HSQUIRRELVM pVM);
};
