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
#include <iostream>
#include "accounts.h"
#include "letter.h"

int main()
{
	std::cout << "Testing sending email...\n";
	Accounts accounts;
	accounts.load("accounts.txt");
	Letter testLetter(&accounts);
	testLetter.from="pay@repetitor.org";
	testLetter.to="fippo@mail.ru";
	testLetter.content="from: pay@repetitor.org\n";
	testLetter.content+="to: fippo@mail.ru\n";
	testLetter.content+="subject: Hi!\n";
	testLetter.content+="\n";
	testLetter.content+="Hello. Testing mail system.\n";
	testLetter.content+="\n.\n";
	try
	{
	testLetter.sendEmail();
	}
	catch (...)
	{
		std::cout << "ERROR while sending!\n";
		return -1;
	}
	std::cout << "Message sended!\n";
	
}
