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
#include "letter.h"

void Letter::sendEmail()
{
	log_->debug("Trying to send e-mail..");
	if ((from=="") || (to=="") || (content==""))
	{
		log_->error("Something is empty!");
		throw ERR_ERROR;
	}
	
	log_->debug("Checking for null in accounts..");
	
	if (accounts_==NULL)
	{
		log_->error("accounts_=NULL!");
		throw ERR_ERROR;
	}
	log_->debug("Loading accs..");
	
	Account account=accounts_->getAccount(from);
	
	log_->debug("Accounts loaded ..");
	
	// 		Отправка письма
	int sock;
	struct sockaddr_in addr;
	struct addrinfo hints, *res;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		log_->error("Socket error!");
		throw ERR_NETWORK;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(account.port);
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;
	getaddrinfo(account.server.c_str(), NULL, &hints, &res);
	
	addr.sin_addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
	
	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		log_->error("Failed to connect to " + account.server);
		throw ERR_NETWORK;
	}

	std::string cmd,resp;
	char buf[1024];
	int l;
	
	recv(sock, buf, sizeof(buf), 0);
	//cout << buf << "\n";
	
	cmd="EHLO " + account.getDomain() + "\n";
	//cout << ">> " << cmd << "\n";
	send(sock, cmd.c_str(), cmd.length(), 0);
	
	l=recv(sock, buf, sizeof(buf), 0);	buf[l]=0;
	//cout << "<< " << buf << "\n";
	
	
	cmd="AUTH LOGIN\n";
	//cout << ">> " << cmd << "\n";
	send(sock, cmd.c_str(), cmd.length(), 0);
	
	l=recv(sock, buf, sizeof(buf), 0); buf[l]=0;
	//cout << "<< " << buf << "\n";
	
	
	cmd=base64_encode(account.login) + "\n";
	//cout << ">> " << cmd << "\n";
	send(sock, cmd.c_str(), cmd.length(), 0);
	
	l=recv(sock, buf, sizeof(buf), 0); buf[l]=0;
	//cout << "<< " << buf << "\n";
	
	
	cmd=base64_encode(account.password) + "\n";
	//cout << ">> " << cmd << "\n";
	send(sock, cmd.c_str(), cmd.length(), 0);
	
	l=recv(sock, buf, sizeof(buf), 0); buf[l]=0;
	//cout << "<< " << buf << "\n";

	
	cmd="MAIL FROM: <" + account.email + ">\n";
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
	resp="";
	if (l>0)
	{
		resp=buf;
	}
	//cout << "<< " << buf << "\n";
	
	cmd="QUIT\n";
	send(sock, cmd.c_str(), cmd.length(), 0);
	
//	recv(sock, buf, sizeof(message), 0);

//	printf(buf);
	close(sock);
	
	//std::cout << "Server response: " << resp << "\n";
	log_->debug("Server response: " + resp);
	
	if (resp.substr(0,3)!="250")
	{
		log_->error("Invalid response!");
		throw ERR_NETWORK;
	}

}

/*
Letter::Letter(Letter &letter)
{
	accounts_=letter.accounts_;
	log_=letter.log_;
	accepting=letter.accepting;
}
*/
