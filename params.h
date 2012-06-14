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
#include <fstream>
#include "misc.h"
#include "exception.h"

class Params
{
public:
	void load(const std::string &fname);	// Загрузка параметров из файла
	size_t getServerPort();					// порт сервера
	size_t getLogLevel();					// уровень логов
	const std::string& getLogFile();		// имя файла логов
	const std::string& getAccountFile();	// имя файла с аккаунтами
private:
	size_t serverPort;			// порт сервера
	size_t logLevel;			// уровень логов
	std::string logFile;		// имя файла логов
	std::string accountFile;	// имя файла с аккаунтами
};
