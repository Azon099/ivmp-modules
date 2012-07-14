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

namespace
	Data
{
	struct
		Group
	{
		int
			groupID;

		std::map<int, bool>
			bots;
	};

	struct
		Queue
	{
		int
			id,
			type;

		std::vector<std::string>
			buffer;
	};
};

class
	CCore
{
	public:
		static std::list<HSQUIRRELVM>
			sq_list;

		static boost::mutex
			mutex;

		static std::vector<Data::Group>
			groups;

		static std::queue<Data::Queue>
			queue;
};
