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
#include "accounts.h"

//------------------------------------------------------------------------------------------
void Accounts::load(const std::string &fname)
{
	std::ifstream in(fname);
	std::string tmp;
	if (!in)
	{
		throw ERR_FILE_READ;
	}
	while (!in.eof())
	{
		getline(in,tmp);		// считываем строку
		if (tmp.length()==0) continue;
		if (tmp[0]=='#') continue;		// пропускаем пустые строки и комментарии
		Strings strings=split(tmp,':');	// разделяем строку на части
		if (strings.size()!=5)
		{							// если в строке не 5 частей, выкидываем ошибку
			throw ERR_FILE_READ;
		}
		Account account;
		account.email=strings[0];
		account.server=strings[1];
		account.port=atoi(strings[2]);
		account.login=strings[3];
		account.password=strings[4];
		accounts_.push_back(account);
	}
	in.close();
	if (accounts_.size()==0)
	{
		throw ERR_ERROR;
	}
}

//------------------------------------------------------------------------------------------
Account Accounts::getAccount(const std::string &email)
{
	if (accounts_.size()==0)
	{
		throw ERR_ERROR;
	}
	for (auto i=accounts_.begin();i<accounts_.end();i++)
	{
		if (email==(*i).email)
		{
			return *i;
		}
	}
	return accounts_[0];
}
