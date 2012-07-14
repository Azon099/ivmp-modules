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

#ifdef MAX_BUFFER
#undef MAX_BUFFER
#endif

#define MAX_BUFFER (4096)

namespace
	Data
{
	struct
		Channel
	{
		bool
			complete;

		std::string
			channel;

		std::vector<std::string>
			names;
	};

	struct
		Main
	{
		bool
			connected,
			ssl;

		int
			botID,
			groupID,
			port;

		SOCKET
			clientSocket;

		SSL_CTX
			* sslContext;

		SSL
			* sslSocket;

		std::string
			localIP,
			nickName,
			realName,
			server,
			userName;
	};
};

class
	CIRC
{
	public:
		CIRC() {}

		CIRC(const CIRC & newIRC)
		{
			main = newIRC.main;
		}

		void
			createThread();

		void
			eraseBot();

		void
			transmitMessage(std::string message);

		Data::Main
			main;

		std::list<Data::Channel>
			channels;
	private:

		void
			initiateConnection();

		void
			parseBuffer(std::string buffer);

		void
			receiveData();
};

extern std::list<CIRC>
	gIRC;
