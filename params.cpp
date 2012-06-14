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
#include "params.h"

//------------------------------------------------------------------------------------------
void Params::load(const std::string &fname)		// Загрузка параметров из файла
{
	std::ifstream in(fname);
	std::string tmp;
	if (!in) throw ERR_FILE_READ;
	while (!in.eof())				// пока не конец файла
	{
		getline(in,tmp);		// считываем строку
		if (tmp.length()==0) continue;
		if (tmp[0]=='#') continue;		// пропускаем пустые строки и комментарии
		Strings strings=split(tmp,'=');	// разделяем строку (до "равно", и после)
		if (strings.size()!=2)
		{								// если количество частей не равно двум, кидаем ошибку
			throw ERR_FILE_READ;
		}
		if (strings[0]=="port")			// обрабатываем параметры, заданные в конфиге
		{
			serverPort=atoi(strings[1]);
		} else
		if (strings[0]=="loglevel")
		{
			logLevel=atoi(strings[1]);
		} else
		if (strings[0]=="logfile")
		{
			logFile=strings[1];
		} else
		if (strings[0]=="accounts")
		{
			accountFile=strings[1];
		}
	}
	if ((serverPort<1) || (serverPort>65535))
	{									// если порт вне диапазона, кидаем ошибку
		throw ERR_FILE_READ;
	}
	if (logLevel>4)
	{									// если уровень логов вне диапазона, кидаем ошибку
		throw ERR_FILE_READ;
	}
	if ((logLevel!=0) && (logFile==""))
	{									// если файл логов не задан, кидаем ошибку
		throw ERR_FILE_READ;
	}
	if (accountFile=="")
	{									// если файл с аккаунтами не задан, кидаем ошибку
		throw ERR_FILE_READ;
	}
	in.close();		// закрываем файл
}

//------------------------------------------------------------------------------------------
size_t Params::getServerPort()
{
	return serverPort;
}

//------------------------------------------------------------------------------------------
size_t Params::getLogLevel()
{
	return logLevel;
}

//------------------------------------------------------------------------------------------
const std::string& Params::getLogFile()
{
	return logFile;
}

//------------------------------------------------------------------------------------------
const std::string& Params::getAccountFile()
{
	return accountFile;
}
