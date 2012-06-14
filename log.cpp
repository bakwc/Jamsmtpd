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
#include "log.h"

//------------------------------------------------------------------------------------------
Log::Log(Params *params):params_(params)
{
	if (params->getLogLevel()!=0)
		{
		outFile_.open(params->getLogFile());
		if (!outFile_)
		{
			throw ERR_FILE_WRITE;
		}
	}
}

//------------------------------------------------------------------------------------------
Log::~Log()
{
	if (params_->getLogLevel()!=0)
	{
		outFile_.close();
	}
}

//------------------------------------------------------------------------------------------
void Log::debug(const std::string &str)
{
	size_t logLevel=params_->getLogLevel();
	if (logLevel==4)
	{
		outFile_ << "DEBUG: " << str << "\n";
		outFile_.flush();
	}
}

//------------------------------------------------------------------------------------------
void Log::error(const std::string &str)
{
	size_t logLevel=params_->getLogLevel();
	if ((logLevel==2) || (logLevel==3) || (logLevel==4))
	{
		outFile_ << "ERROR: " << str << "\n";
		outFile_.flush();
	}
}

//------------------------------------------------------------------------------------------
void Log::info(const std::string &str)
{
	size_t logLevel=params_->getLogLevel();
	if ((logLevel==1) || (logLevel==3) || (logLevel==4))
	{
		outFile_ << "INFO: " << str << "\n";
		outFile_.flush();
	}
}
