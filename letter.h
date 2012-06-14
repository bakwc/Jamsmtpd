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

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <cstring>
#include "log.h"
#include "misc.h"
#include "accounts.h"
#include "account.h"
#include "exception.h"

using namespace std;

class Letter
{
public:
	Letter(Accounts *accounts, Log *log):accounts_(accounts), log_(log){ accepting=false;}
	//Letter(Letter &letter);
	void sendEmail();	// Отправка письма
	std::string from,to,content; // Отправитель, получатель, содержимое
	bool accepting;
private:
	Accounts *accounts_;
	Log *log_;
};
