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
#include <vector>
#include <sstream>
#include <cmath>
#include "exception.h"

typedef std::vector<std::string> Strings;

std::string itoa(int num);									 // число в строку
int atoi(std::string const &str);							 // строку в число
Strings split(const std::string &str,const char letter=' '); // раздел€ет строку
std::string base64_encode(const std::string &data);			 //  одирует строку в base64
std::string extractEmail(const std::string &str);			 // »звлекает e-mail из строки 
