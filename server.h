/*
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
#pragma once

#include <algorithm>
#include <map>
//#include <set>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include "accounts.h"
#include "params.h"
#include "log.h"
#include "letter.h"

class Server
{
public:
	Server(Params *params, Accounts *accounts, Log *log):params_(params),accounts_(accounts),log_(log) {}
	void run();
private:
	void receivedStr(int from, const std::string& req);
	void sendStr(int to, const std::string& str);
	void clientConnected(int client);
	void disconnectClient(int client);
	Params *params_;
	Accounts *accounts_;
	Log *log_;
	std::map<int,Letter> clients_;
	//set<int> clients;
};
