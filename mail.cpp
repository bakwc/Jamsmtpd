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
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <set>
#include <vector>
#include <iostream>
#include <cstring>
#include <regex>
#include <math.h>
#include <cstdint>
#include <cstdlib>
#include <netdb.h>


using namespace std;


static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

std::string base64_encode(const std::string &data)
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


std::string extractEmail(const std::string &str)
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

struct Account
{
	std::string email,pass,host,domain;
	size_t port;
};

struct Accounts
{
private:
	std::vector<Account> accs_;
public:
	void load(const std::string& fname)
	{
		Account tmp;
		tmp.email="pay@repetitor.org";
		tmp.domain="repetitor.org";
		tmp.host="smtp.yandex.ru";
		tmp.pass="ONLE84naAkei";
		tmp.port=25;
		accs_.push_back(tmp);
	}
	Account getAcc(const std::string& email)
	{
		Account res;
		for (auto i=accs_.begin();i<accs_.end();i++)
		{
			if (email==(*i).email)
				res=*i;
		}
		return res;
	}
};

struct ClientInfo
{
	std::string from,to,content;
	bool accepting;
	Accounts *accounts;
	void clear()
	{
		from="";
		to="";
		content="";
		accepting=false;
	}
	void doSend()
	{
		if ((from=="") || (to=="") || (content==""))
		{
			cout << "ERROR! Unable to send, need more information.\n";
			return;
		}
		Account acc=accounts->getAcc(from);
		
		if (acc.host=="")
		{
			cout << "ERROR! " + from + " - account not found!\n";
			return;
		}
		
		
		// 		Отправка письма
		int sock;
		struct sockaddr_in addr;
		struct addrinfo hints, *res;

		sock = socket(AF_INET, SOCK_STREAM, 0);
		/* if(sock < 0)
		{
			perror("socket");
			exit(1);
		} */

		addr.sin_family = AF_INET;
		addr.sin_port = htons(acc.port);
		//addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		//addr.sin_addr.s_addr=gethostbyname(acc.host.c_str());
		
		memset(&hints, 0, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = AF_INET;
		getaddrinfo(acc.host.c_str(), NULL, &hints, &res);
		
		addr.sin_addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
		
		if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{
			perror("connect");
			exit(2);
		}

		std::string cmd;
		char buf[1024];
		int l;
		
		recv(sock, buf, sizeof(buf), 0);
		cout << buf << "\n";
		
		
		cmd="EHLO " + acc.domain + "\n";
		//cout << ">> " << cmd << "\n";
		send(sock, cmd.c_str(), cmd.length(), 0);
		
		l=recv(sock, buf, sizeof(buf), 0);	buf[l]=0;
		//cout << "<< " << buf << "\n";
		
		
		cmd="AUTH LOGIN\n";
		//cout << ">> " << cmd << "\n";
		send(sock, cmd.c_str(), cmd.length(), 0);
		
		l=recv(sock, buf, sizeof(buf), 0); buf[l]=0;
		//cout << "<< " << buf << "\n";
		
		
		cmd=base64_encode(acc.email) + "\n";
		//cout << ">> " << cmd << "\n";
		send(sock, cmd.c_str(), cmd.length(), 0);
		
		l=recv(sock, buf, sizeof(buf), 0); buf[l]=0;
		//cout << "<< " << buf << "\n";
		
		
		cmd=base64_encode(acc.pass) + "\n";
		//cout << ">> " << cmd << "\n";
		send(sock, cmd.c_str(), cmd.length(), 0);
		
		l=recv(sock, buf, sizeof(buf), 0); buf[l]=0;
		//cout << "<< " << buf << "\n";

		
		cmd="MAIL FROM: <" + acc.email + ">\n";
		//cout << ">> " << cmd << "\n";
		send(sock, cmd.c_str(), cmd.length(), 0);
		
		l=recv(sock, buf, sizeof(buf), 0); buf[l]=0;
		//cout << "<< " << buf << "\n";
		
		
		cmd="RCPT TO: <" + to + ">\n";
		//cout << ">> " << cmd << "\n";
		send(sock, cmd.c_str(), cmd.length(), 0);
		
		l=recv(sock, buf, sizeof(buf), 0); buf[l]=0;
		//cout << "<< " << buf << "\n";		
		
		
		cmd="DATA\n";
		//cout << ">> " << cmd << "\n";
		send(sock, cmd.c_str(), cmd.length(), 0);
		
		l=recv(sock, buf, sizeof(buf), 0); buf[l]=0;
		//cout << "<< " << buf << "\n";
		
		
		cmd=content;
		send(sock, cmd.c_str(), cmd.length(), 0);
		
		l=recv(sock, buf, sizeof(buf), 0); buf[l]=0;
		//cout << "<< " << buf << "\n";
		
		cmd="QUIT\n";
		send(sock, cmd.c_str(), cmd.length(), 0);
		
		//recv(sock, buf, sizeof(message), 0);

		//printf(buf);
		close(sock);

		/*
		cout << "Connect to " + acc.host + "\n";
		cout << "EHLO " + acc.domain + "\n";
		cout << "AUTH LOGIN\n";
		cout << base64_encode(acc.email) + "\n";
		cout << base64_encode(acc.pass) + "\n";
		cout << "MAIL FROM: <" + acc.email + ">\n";
		cout << "RCPT TO: <" + to + ">\n";
		cout << "DATA\n\n";
		cout << content;
		cout << "QUIT";
		*/
	}
};

int main()
{
    int listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;
	set<int> clients;
	vector<ClientInfo> users;
	Accounts accounts;
	accounts.load("accounts.cfg");
	
	//std::cout << "pay@repetitor.org: " << base64_encode("pay@repetitor.org") << "\n";
	

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    
    fcntl(listener, F_SETFL, O_NONBLOCK);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(35);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 2);
    
    clients.clear();

    while(1)
    {
        // Заполняем множество сокетов
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(listener, &readset);

        for(set<int>::iterator it = clients.begin(); it != clients.end(); it++)
            FD_SET(*it, &readset);

        // Задаём таймаут
        timeval timeout;
        timeout.tv_sec = 15;
        timeout.tv_usec = 0;

        // Ждём события в одном из сокетов
        int mx = max(listener, *max_element(clients.begin(), clients.end()));
        select(mx+1, &readset, NULL, NULL, &timeout);


        // Определяем тип события и выполняем соответствующие действия
        if(FD_ISSET(listener, &readset))
        {
            // Поступил новый запрос на соединение, используем accept
            int sock = accept(listener, NULL, NULL);
            if(sock < 0)
            {
                perror("accept");
                exit(3);
            }
            
            fcntl(sock, F_SETFL, O_NONBLOCK);

            clients.insert(sock);
			
			std::string answ="220 Ready to serve\n";
			send(sock, answ.c_str(), answ.length(), 0);
			
			if (sock>users.size())
				users.resize(sock+1);
			ClientInfo newUser;
			newUser.clear();
			newUser.accounts=&accounts;
			users[sock]=newUser;
        }

        for(set<int>::iterator it = clients.begin(); it != clients.end(); it++)
        {
            if(FD_ISSET(*it, &readset))
            {
                // Поступили данные от клиента, читаем их
				std::string answ,req;
                bytes_read = recv(*it, buf, 1024, 0);
				buf[bytes_read]=0;
                if(bytes_read <= 0)
                {
                    // Соединение разорвано, удаляем сокет из множества
					cout << "Client disconnected\n\n";
					users[*it].clear();
                    close(*it);
                    clients.erase(*it);
                    continue;
                }

				req=buf;		
				
				//std::cout << "CMD: " << req << "\n-----\n";
				
				if (users[*it].accepting==true)
				{
					users[*it].content+=req;
					std::string ending;
					ending=users[*it].content.substr(users[*it].content.length()-3,3);
					if (ending=="\n.\n")
					{
						//cout << "Client finished sending data\n\n";
						users[*it].doSend();
						users[*it].accepting=false;
					}
				} else
				if ( (req.substr(0,4)=="EHLO") || (req.substr(0,4)=="HELO")  )
				{
					//cout << "Client said helo\n";
					answ="250 Ok!\n";
					send(*it, answ.c_str(), answ.length(), 0);
				} else
				
				if (req.substr(0,10)=="MAIL FROM:")
				{
					std::string email=extractEmail(req);
					//cout << "Client from <"+email+">\n";
					users[*it].from=email;
					answ="250 Ok!\n";
					send(*it, answ.c_str(), answ.length(), 0);
				} else

				if (req.substr(0,8)=="RCPT TO:")
				{
					std::string email=extractEmail(req);
					//cout << "Client to <"+email+">\n";
					users[*it].to=email;
					answ="250 Ok!\n";
					send(*it, answ.c_str(), answ.length(), 0);
				} else

				if (req.substr(0,4)=="DATA")
				{
					//cout << "Client sending data\n";
					answ="354 ok!\n";
					send(*it, answ.c_str(), answ.length(), 0);
					users[*it].accepting=true;
				} else

				if (req.substr(0,4)=="QUIT")
				{
					//cout << "Client quit\n\n";
					users[*it].clear();
					close(*it);
                    clients.erase(*it);
				}

            }
        }
    }
    
    return 0;
}