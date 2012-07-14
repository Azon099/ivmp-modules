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
#include "CError.h"

std::string
	CError::getLastError()
{
	std::string
		outputBuffer;
	#ifdef WIN32
		LPTSTR
			inputBuffer;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&inputBuffer, 0, NULL);
		if (inputBuffer != NULL)
		{
			outputBuffer = inputBuffer;
			outputBuffer.resize(outputBuffer.length() - 2);
		}
	#else
		outputBuffer = strerror(errno);
	#endif
	return outputBuffer;
}

std::string
	CError::getSSLError()
{
	std::string
		outputBuffer = ERR_error_string(ERR_get_error(), NULL);
	return outputBuffer;
}
