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
#include "misc.h"

//------------------------------------------------------------------------------------------
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};	// Таблица для кодирования в base64
static int mod_table[] = {0, 2, 1};

//------------------------------------------------------------------------------------------
std::string itoa(int num)	// число в строку
{
	std::stringstream out;
	out << num;
	return out.str();
}

//------------------------------------------------------------------------------------------
int atoi(std::string const &str)  // строку в число
{
	std::istringstream stream(str);
	int number = 0;
	stream >> number;
	if (stream.fail()) throw ERR_STR_TO_NUM;
	return number;
}

//------------------------------------------------------------------------------------------
Strings split(const std::string &str,const char letter)	// разделяет строку
{
	Strings args;
    int n=str.find(letter);
    std::string tmp,str2=str;
    while (n>=0)
    {
        tmp=str2.substr(0,n);
        str2.erase(0,n+1);
        args.push_back(tmp);
        n=str2.find(letter);
    }
    args.push_back(str2);
	return args;
}

//------------------------------------------------------------------------------------------
std::string base64_encode(const std::string &data)	// Кодирует строку в base64
{
    size_t output_length = (size_t) (4.0 * ceil((double) data.length() / 3.0));
	std::string encoded_data;
    for (int i = 0; i < data.length();) {
        uint32_t octet_a = i < data.length() ? data[i++] : 0;
        uint32_t octet_b = i < data.length() ? data[i++] : 0;
        uint32_t octet_c = i < data.length() ? data[i++] : 0;
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
		encoded_data.push_back(encoding_table[(triple >> 3 * 6) & 0x3F]);
		encoded_data.push_back(encoding_table[(triple >> 2 * 6) & 0x3F]);
		encoded_data.push_back(encoding_table[(triple >> 1 * 6) & 0x3F]);
		encoded_data.push_back(encoding_table[(triple >> 0 * 6) & 0x3F]);
    }
    for (int i = 0; i < mod_table[data.length() % 3]; i++)
        encoded_data[output_length - 1 - i] = '=';
    return encoded_data;
}

//------------------------------------------------------------------------------------------
std::string extractEmail(const std::string &str)	// Извлекает e-mail из строки
{
	size_t emailPos=-1,emailBegin=0,emailEnd=str.length();
	for (size_t i=0;i<str.length();i++)
		if (str[i]=='@') emailPos=i;
	if (emailPos==-1) return "";
	for (size_t i=emailPos;i>=0;i--)
		if ((str[i]==' ') || (str[i]=='<'))
		{
			emailBegin=i;
			break;
		}
	for (size_t i=emailPos;i<str.length();i++)
		if ((str[i]==' ') || (str[i]=='>') || (str[i]=='\n')  || (str[i]=='\t')  || (str[i]==';')  || (str[i]==','))
		{
			emailEnd=i;
			break;
		}
	return str.substr(emailBegin+1,emailEnd-emailBegin-1);
}