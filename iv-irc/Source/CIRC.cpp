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
#include "CError.h"
#include "CIRC.h"

std::list<CIRC>
	gIRC;

void
	CIRC::createThread()
{
	boost::thread
		initiateConnectionThread(&CIRC::initiateConnection, this);
}

void
	CIRC::eraseBot()
{
	boost::mutex::scoped_lock
		lock(CCore::mutex);
	shutdown(main.clientSocket, 2);
	closesocket(main.clientSocket);
	if (main.ssl)
	{
		SSL_shutdown(main.sslSocket);
		SSL_CTX_free(main.sslContext);
		SSL_free(main.sslSocket);
	}
	Data::Queue
		newQueue;
	newQueue.id = main.botID;
	newQueue.type = 1;
	CCore::queue.push(newQueue);
	for (std::list<CIRC>::iterator i = gIRC.begin(); i != gIRC.end(); ++i)
	{
		if ((* i).main.botID == main.botID)
		{
			gIRC.erase(i);
			break;
		}
	}
}

void
	CIRC::initiateConnection()
{
	main.clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (main.clientSocket == INVALID_SOCKET)
	{
		LogPrintf("*** IRC Plugin: Error creating socket (%s)", CError::getLastError().c_str());
		eraseBot();
		return;
	}
	if (main.ssl)
	{
		main.sslContext = SSL_CTX_new(SSLv23_client_method());
		main.sslSocket = SSL_new(main.sslContext);
		if (main.sslSocket == NULL)
		{
			LogPrintf("*** IRC Plugin: Error creating SSL socket (%s)", CError::getSSLError().c_str());
			eraseBot();
			return;
		}
	}
	struct addrinfo
		* result;
	if (!getaddrinfo(main.server.c_str(), NULL, NULL, &result))
	{
		char
			ip[16];
		sockaddr_in
			clientInfo,
			serverInfo;
		if (!main.localIP.empty())
		{
			clientInfo.sin_addr.s_addr = inet_addr(main.localIP.c_str());
			clientInfo.sin_family = AF_INET;
			clientInfo.sin_port = htons(0);
			if (bind(main.clientSocket, (struct sockaddr *)&clientInfo, (socklen_t)sizeof(clientInfo)) == SOCKET_ERROR)
			{
				LogPrintf("*** IRC Plugin: Error binding socket (%s)", CError::getLastError().c_str());
				eraseBot();
				return;
			}
		}
		if (!getnameinfo(result->ai_addr, result->ai_addrlen, ip, sizeof(ip), NULL, NI_MAXSERV, NI_NUMERICHOST))
		{
			freeaddrinfo(result);
			serverInfo.sin_addr.s_addr = inet_addr(ip);
			serverInfo.sin_family = AF_INET;
			serverInfo.sin_port = htons(main.port);
			if (connect(main.clientSocket, (const sockaddr *)&serverInfo, sizeof(serverInfo)) != SOCKET_ERROR)
			{
				if (main.ssl)
				{
					SSL_set_fd(main.sslSocket, main.clientSocket);
					if (SSL_connect(main.sslSocket) != 1)
					{
						LogPrintf("*** IRC Plugin: Error initiating SSL session (%s)", CError::getSSLError().c_str());
						eraseBot();
						return;
					}
				}
				std::string
					sendBuffer = boost::str(boost::format("USER %1% 0 * :%2%\r\nNICK %3%\r\n") % main.userName % main.realName % main.nickName);
				transmitMessage(sendBuffer);
				receiveData();
				return;
			}
			else
			{
				LogPrintf("*** IRC Plugin: Could not connect to %s:%d (%s)", main.server.c_str(), main.port, CError::getLastError().c_str());
				eraseBot();
				return;
			}
		}
		freeaddrinfo(result);
	}
	LogPrintf("*** IRC Plugin: Could not resolve server address (%s)", CError::getLastError().c_str());
	eraseBot();
}

void
	CIRC::parseBuffer(std::string buffer)
{
	std::string
		command,
		host,
		message,
		parameters,
		user,
		tempBuffer = buffer;
	size_t
		location = tempBuffer.find(" :");
	if (location != std::string::npos)
	{
		message = buffer.substr(location + 2);
		tempBuffer.erase(location, buffer.length());
	}
	std::vector<std::string>
		splitBuffer;
	boost::algorithm::split(splitBuffer, tempBuffer, boost::algorithm::is_any_of(" "));
	for (std::vector<std::string>::iterator i = splitBuffer.begin(); i != splitBuffer.end(); ++i)
	{
		if (i == splitBuffer.begin())
		{
			if (!(* i).find_first_of(':'))
			{
				(* i).erase(0, 1);
				if ((* i).find('!') != std::string::npos)
				{
					std::vector<std::string>
						parsedBuffer;
					boost::algorithm::split(parsedBuffer, * i, boost::algorithm::is_any_of("!"));
					if (parsedBuffer.size() == 2)
					{
						user = parsedBuffer.at(0);
						host = parsedBuffer.at(1);
					}
				}
			}
			else
			{
				command = * i;
			}
		}
		else if (command.empty())
		{
			command = * i;
		}
		else
		{
			if (parameters.empty())
			{
				parameters = * i;
			}
			else
			{
				parameters += " " + * i;
			}
		}
	}
	if (!message.empty())
	{
		if (command.compare("353") != 0)
		{
			boost::algorithm::replace_all(message, "%", "");
		}
	}
	if (!command.compare("PING"))
	{
		std::string
			sendBuffer = buffer;
		boost::algorithm::replace_first(sendBuffer, "PING", "PONG");
		sendBuffer.append("\r\n");
		transmitMessage(sendBuffer);
	}
	else if (!command.compare("QUIT"))
	{
		if (!host.empty() && !user.empty())
		{
			if (user.compare(main.nickName) != 0)
			{
				if (message.empty())
				{
					message = "None";
				}
				Data::Queue
					newQueue;
				newQueue.buffer.push_back(message);
				newQueue.buffer.push_back(host);
				newQueue.buffer.push_back(user);
				newQueue.id = main.botID;
				newQueue.type = 4;
				CCore::queue.push(newQueue);
				for (std::list<Data::Channel>::iterator c = channels.begin(); c != channels.end(); ++c)
				{
					for (std::vector<std::string>::iterator n = (* c).names.begin(); n != (* c).names.end(); ++n)
					{
						if ((* n).find(user) != std::string::npos)
						{
							(* c).names.erase(n);
							break;
						}
					}
				}
			}
		}
	}
	else if (!command.compare("JOIN"))
	{
		if (!host.empty() && !user.empty() && !message.empty())
		{
			Data::Queue
				newQueue;
			if (!user.compare(main.nickName))
			{
				newQueue.buffer.push_back(message);
				newQueue.id = main.botID;
				newQueue.type = 2;
			}
			else
			{
				newQueue.buffer.push_back(host);
				newQueue.buffer.push_back(user);
				newQueue.buffer.push_back(message);
				newQueue.id = main.botID;
				newQueue.type = 5;
				for (std::list<Data::Channel>::iterator c = channels.begin(); c != channels.end(); ++c)
				{
					if (!(* c).channel.compare(message))
					{
						bool
							found = false;
						for (std::vector<std::string>::iterator n = (* c).names.begin(); n != (* c).names.end(); ++n)
						{
							if ((* n).find(user) != std::string::npos)
							{
								found = true;
								break;
							}
						}
						if (!found)
						{
							(* c).names.push_back(user);
						}
						break;
					}
				}
			}
			CCore::queue.push(newQueue);
		}
	}
	else if (!command.compare("PART"))
	{
		if (!host.empty() && !user.empty() && !parameters.empty())
		{
			if (message.empty())
			{
				message = "None";
			}
			Data::Queue
				newQueue;
			if (!user.compare(main.nickName))
			{
				newQueue.buffer.push_back(message);
				newQueue.buffer.push_back(parameters);
				newQueue.id = main.botID;
				newQueue.type = 3;
			}
			else
			{
				newQueue.buffer.push_back(message);
				newQueue.buffer.push_back(host);
				newQueue.buffer.push_back(user);
				newQueue.buffer.push_back(parameters);
				newQueue.id = main.botID;
				newQueue.type = 6;
				for (std::list<Data::Channel>::iterator c = channels.begin(); c != channels.end(); ++c)
				{
					if (!(* c).channel.compare(parameters))
					{
						for (std::vector<std::string>::iterator n = (* c).names.begin(); n != (* c).names.end(); ++n)
						{
							if ((* n).find(user) != std::string::npos)
							{
								(* c).names.erase(n);
								break;
							}
						}
						break;
					}
				}
			}
			CCore::queue.push(newQueue);
		}
	}
	else if (!command.compare("KICK"))
	{
		if (!host.empty() && !user.empty() && !parameters.empty())
		{
			if (message.empty())
			{
				message = "None";
			}
			std::vector<std::string>
				splitParameters;
			boost::algorithm::split(splitParameters, parameters, boost::algorithm::is_any_of(" "));
			if (splitParameters.size() == 2)
			{
				Data::Queue
					newQueue;
				if (!splitParameters.at(1).compare(main.nickName))
				{
					newQueue.buffer.push_back(message);
					newQueue.buffer.push_back(splitParameters.at(0));
					newQueue.id = main.botID;
					newQueue.type = 3;
				}
				else
				{
					newQueue.buffer.push_back(message);
					newQueue.buffer.push_back(host);
					newQueue.buffer.push_back(splitParameters.at(1));
					newQueue.buffer.push_back(splitParameters.at(0));
					newQueue.id = main.botID;
					newQueue.type = 6;
					for (std::list<Data::Channel>::iterator c = channels.begin(); c != channels.end(); ++c)
					{
						if (!(* c).channel.compare(splitParameters.at(0)))
						{
							for (std::vector<std::string>::iterator n = (* c).names.begin(); n != (* c).names.end(); ++n)
							{
								if ((* n).find(user) != std::string::npos)
								{
									(* c).names.erase(n);
									break;
								}
							}
							break;
						}
					}
				}
				CCore::queue.push(newQueue);
			}
		}
	}
	else if (!command.compare("NICK"))
	{
		if (!host.empty() && !message.empty() && !user.empty())
		{
			if (user.compare(main.nickName) != 0)
			{
				Data::Queue
					newQueue;
				newQueue.buffer.push_back(host);
				newQueue.buffer.push_back(message);
				newQueue.buffer.push_back(user);
				newQueue.id = main.botID;
				newQueue.type = 7;
				CCore::queue.push(newQueue);
			}
			else
			{
				main.nickName = message;
			}
			for (std::list<Data::Channel>::iterator c = channels.begin(); c != channels.end(); ++c)
			{
				for (std::vector<std::string>::iterator n = (* c).names.begin(); n != (* c).names.end(); ++n)
				{
					if ((* n).find(user) != std::string::npos)
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
								(* n).replace((* n).begin() + 1, (* n).end(), message);
							}
							break;
							default:
							{
								(* n).assign(message);
							}
							break;
						}
						break;
					}
				}
			}
		}
	}
	else if (!command.compare("MODE"))
	{
		if (!parameters.empty() && !host.empty() && !user.empty())
		{
			size_t
				result = parameters.find_first_of(' ');
			if (result != std::string::npos)
			{
				std::string
					modes = parameters.substr(result + 1);
				parameters.erase(result, parameters.length());
				if (user.compare(main.nickName) != 0)
				{
					Data::Queue
						newQueue;
					newQueue.buffer.push_back(modes);
					newQueue.buffer.push_back(host);
					newQueue.buffer.push_back(user);
					newQueue.buffer.push_back(parameters);
					newQueue.id = main.botID;
					newQueue.type = 8;
					CCore::queue.push(newQueue);
				}
				result = modes.find_first_of(' ');
				if (result != std::string::npos)
				{
					modes.erase(result, modes.length());
					if (modes.find_first_of("vhoauq") != std::string::npos)
					{
						std::string
							sendBuffer = boost::str(boost::format("NAMES %1%\r\n") % parameters);
						transmitMessage(sendBuffer);
					}
				}
			}
		}
	}
	else if (!command.compare("TOPIC"))
	{
		if (!host.empty() && !user.empty() && !parameters.empty())
		{
			if (message.empty())
			{
				message = "None";
			}
			if (user.compare(main.nickName) != 0)
			{
				Data::Queue
					newQueue;
				newQueue.buffer.push_back(message);
				newQueue.buffer.push_back(host);
				newQueue.buffer.push_back(user);
				newQueue.buffer.push_back(parameters);
				newQueue.id = main.botID;
				newQueue.type = 9;
				CCore::queue.push(newQueue);
			}
		}
	}
	else if (!command.compare("PRIVMSG"))
	{
		if (!message.empty() && !host.empty() && !user.empty() && !parameters.empty())
		{
			if (parameters.at(0) == '#' || parameters.at(0) == '&')
			{
				for (std::vector<Data::Group>::iterator g = CCore::groups.begin(); g != CCore::groups.end(); ++g)
				{
					if ((* g).groupID == main.groupID)
					{
						if ((* g).bots.size() > 1)
						{
							std::map<int, bool>::iterator
								s = (* g).bots.begin();
							if ((* s).first != main.botID)
							{
								return;
							}
						}
						break;
					}
				}
			}
			if (user.compare(main.nickName) != 0)
			{
				Data::Queue
					newQueue;
				newQueue.buffer.push_back(message);
				newQueue.buffer.push_back(host);
				newQueue.buffer.push_back(user);
				newQueue.buffer.push_back(parameters);
				newQueue.id = main.botID;
				newQueue.type = 10;
				CCore::queue.push(newQueue);
			}
		}
	}
	else if (!command.compare("NOTICE"))
	{
		if (!message.empty() && !host.empty() && !user.empty() && !parameters.empty())
		{
			if (user.compare(main.nickName) != 0)
			{
				Data::Queue
					newQueue;
				newQueue.buffer.push_back(message);
				newQueue.buffer.push_back(host);
				newQueue.buffer.push_back(user);
				newQueue.buffer.push_back(parameters);
				newQueue.id = main.botID;
				newQueue.type = 11;
				CCore::queue.push(newQueue);
			}
		}
	}
	else if (!command.compare("001"))
	{
		main.connected = true;
		Data::Queue
			newQueue;
		newQueue.id = main.botID;
		newQueue.type = 0;
		CCore::queue.push(newQueue);
	}
	else if (!command.compare("353"))
	{
		if (!message.empty() && !parameters.empty())
		{
			std::string
				channel;
			std::vector<std::string>
				splitParameters;
			boost::algorithm::split(splitParameters, parameters, boost::algorithm::is_any_of(" = "));
			channel = splitParameters.back();
			splitParameters.clear();
			boost::algorithm::trim(message);
			boost::algorithm::split(splitParameters, message, boost::algorithm::is_any_of(" "));
			for (std::list<Data::Channel>::iterator c = channels.begin(); c != channels.end(); ++c)
			{
				if (!(* c).channel.compare(channel))
				{
					if ((* c).complete)
					{
						(* c).complete = false;
						(* c).names.clear();
						(* c).names.insert((* c).names.begin(), splitParameters.begin(), splitParameters.end());
						splitParameters.clear();
					}
					else
					{
						for (std::vector<std::string>::iterator p = splitParameters.begin(); p != splitParameters.end(); ++p)
						{
							(* c).names.push_back(* p);
						}
						splitParameters.clear();
					}
					break;
				}
			}
			if (!splitParameters.empty())
			{
				Data::Channel
					newChannel;
				newChannel.channel = channel;
				newChannel.complete = false;
				newChannel.names.insert(newChannel.names.begin(), splitParameters.begin(), splitParameters.end());
				channels.push_back(newChannel);
			}
		}
	}
	else if (!command.compare("366"))
	{
		if (!message.empty() && !parameters.empty())
		{
			std::string
				channel;
			std::vector<std::string>
				splitParameters;
			boost::algorithm::split(splitParameters, parameters, boost::algorithm::is_any_of(" "));
			channel = splitParameters.back();
			for (std::list<Data::Channel>::iterator c = channels.begin(); c != channels.end(); ++c)
			{
				if (!(* c).channel.compare(channel))
				{
					if (!(* c).complete)
					{
						(* c).complete = true;
					}
					break;
				}
			}
		}
	}
}

void
	CIRC::receiveData()
{
	char
		buffer[MAX_BUFFER];
	int
		len = 0;
	while (true)
	{
		if (main.ssl)
		{
			len = SSL_read(main.sslSocket, buffer, MAX_BUFFER);
		}
		else
		{
			len = recv(main.clientSocket, buffer, MAX_BUFFER, 0);
		}
		if (len < 1)
		{
			eraseBot();
			return;
		}
		boost::mutex::scoped_lock
			lock(CCore::mutex);
		std::string
			messageBuffer = buffer;
		std::vector<std::string>
			splitBuffer;
		messageBuffer.resize(len);
		boost::algorithm::split(splitBuffer, messageBuffer, boost::algorithm::is_any_of("\r\n"));
		for (std::vector<std::string>::iterator i = splitBuffer.begin(); i != splitBuffer.end(); ++i)
		{
			if (!(* i).empty())
			{
				Data::Queue
					newQueue;
				if ((* i).length() > MAX_BUFFER / 8)
				{
					std::string
						tempBuffer = * i;
					tempBuffer.resize(MAX_BUFFER / 8);
					newQueue.buffer.push_back(tempBuffer);
				}
				else
				{
					newQueue.buffer.push_back(* i);
				}
				newQueue.id = main.botID;
				newQueue.type = 12;
				CCore::queue.push(newQueue);
				parseBuffer(* i);
			}
		}
	}
}

void
	CIRC::transmitMessage(std::string message)
{
	if (main.ssl)
	{
		SSL_write(main.sslSocket, message.c_str(), message.length());
	}
	else
	{
		send(main.clientSocket, message.c_str(), message.length(), MSG_NOSIGNAL);
	}
}
