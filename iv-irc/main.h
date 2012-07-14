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

#define MAX_BUFFER (512)
#define PLUGIN_VERSION "1.3.2"

#include <SDK.h>

#ifdef WIN32
	#define BOOST_ALL_NO_LIB
	#define MSG_NOSIGNAL (0)
	#include <ws2tcpip.h>
#else
	#define closesocket(s) close(s)
	#define SOCKET int
	#define INVALID_SOCKET (SOCKET)(~0)
	#define SOCKET_ERROR (-1)
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <sys/socket.h>
	#include <sys/types.h>
#endif
#include <queue>
#include "Libraries/boost/algorithm/string.hpp"
#include "Libraries/boost/format.hpp"
#include "Libraries/boost/thread.hpp"
#include "Libraries/openssl/err.h"
#include "Libraries/openssl/ssl.h"
