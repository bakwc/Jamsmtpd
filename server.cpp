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
#include "server.h"

//------------------------------------------------------------------------------------------
void Server::run()
{
	int listener;
	struct sockaddr_in addr;
	char buf[1024];
	int bytes_read;
	std::string request;
	
	listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        throw ERR_NETWORK;
    }
	
	fcntl(listener, F_SETFL, O_NONBLOCK);
	
	addr.sin_family = AF_INET;
    addr.sin_port = htons(params_->getServerPort());
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
		throw ERR_NETWORK;
    }
	
	listen(listener, 2);
	
	clients_.clear();
	
	while(1)
    {
        // Заполняем множество сокетов
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(listener, &readset);

        for(auto it = clients_.begin(); it != clients_.end(); it++)
			{
				FD_SET((*it).first, &readset);
			}
		
		 // Задаём таймаут
        timeval timeout;
        timeout.tv_sec = 15;
        timeout.tv_usec = 0;
		
		// Ждём события в одном из сокетов
		int mx=-5;
		
		for (auto it=clients_.begin();it!=clients_.end();it++)
		{
			if ((*it).first>mx)
			{
				mx=(*it).first;
			}
		}

		mx=max(listener,mx);
        //int mx = max(listener, *max_element(clients_.begin(), clients_.end())  );
        select(mx+1, &readset, NULL, NULL, &timeout);
		
		// Определяем тип события и выполняем соответствующие действия
        if(FD_ISSET(listener, &readset))
        {
            // Поступил новый запрос на соединение, используем accept
            int sock = accept(listener, NULL, NULL);
            if(sock < 0)
            {
                log_->error("Socket accept error!");
				continue;
            }
            clientConnected(sock);
        }
		
		for(auto it = clients_.begin(); it != clients_.end(); it++)
        {
			//log_->debug("SOCK: "+itoa((*it).first));
			
			/*
			for (int i=0;i<10;i++)
				if(FD_ISSET(i, &readset)!=0)
					log_->debug("FD_SET-Y: "+itoa(i));
				else
					log_->debug("FD_SET-N: "+itoa(i)); 
			*/
            if(FD_ISSET( ((*it).first), &readset) !=0  )
            {
				log_->debug("Smthing here!");
                // Поступили данные от клиента, читаем их
                bytes_read = recv((*it).first, buf, 1024, 0);
				
				log_->debug("Read bytes: " + itoa(bytes_read));
				
				buf[bytes_read]=0;
                if(bytes_read <= 0)
                {
					//cout << "Client disconnected\n\n";
					disconnectClient((*it).first);
                    continue;
                }
			
				request=buf;
				log_->debug(request);
				
				receivedStr((*it).first, request);
			}
		}

	}

}

//------------------------------------------------------------------------------------------
void Server::receivedStr(int from, const std::string& req)
{
	log_->debug("receivedStr: <"+req+">");
	
	auto frm=clients_.find(from);

	//if (frm->second.accepting==true)
	if (frm->second.accepting==true)
	{
		log_->debug("Accepting data...");
		frm->second.content+=req;
		log_->debug("content: <" + frm->second.content + ">");
		std::string ending;
		//ending="";
		//if (frm->second.content.length()>3)
		//{
		//	ending=frm->second.content.substr(frm->second.content.length()-4,3);
		//}
		//log_->debug("ending: <" + ending + ">");
		//if (ending=="\n.\n")
		//int p=frm->second.content.find("\13\10\46\13\10");
		
		
		if (frm->second.content.length()>5)
		{
			for (int i=frm->second.content.length()-5;i<frm->second.content.length();i++)
				log_->debug("S " + itoa( frm->second.content[i] ));
		}
		
		//log_->debug("FND AT " + itoa(p));
		
		//if (ending.find("\n.\n")!=std::string::npos)
		//if (frm->second.content.find("\n.\n") == frm->second.content.length()-3)
		
		int l=frm->second.content.length();
		if (l<5) return;
		
		
		
		if ((( frm->second.content[l-5]==13) &&
			( frm->second.content[l-4]==10) &&
			( frm->second.content[l-3]==46) &&
			( frm->second.content[l-2]==13) &&
			( frm->second.content[l-1]==10)) ||
			
			(( frm->second.content[l-4]==10) &&
			( frm->second.content[l-3]==10) &&
			( frm->second.content[l-2]==46) &&
			( frm->second.content[l-1]==10)))
		{
			//cout << "Client finished sending data\n\n";
			log_->debug("Client finished sending data");
			try
			{
				frm->second.sendEmail();
				log_->info("E-Mail sended from "+frm->second.from+" to "+frm->second.to);
				sendStr(from,"250 E-Mail sended!\n");
			}
			catch(...)
			{
				log_->error("Error sending from "+frm->second.from+" to "+frm->second.to);
				sendStr(from,"514 Error while sending e-mail!\n");
			}
			
			frm->second.accepting=false;
		}
	} else
	if ( (req.find("EHLO")==0) || (req.find("HELO")==0)  )
	{
		//cout << "Client said helo\n";
		log_->debug("Client said helo");
		sendStr(from,"250 Ok!\n");
	} else
	
	if (req.find("MAIL FROM:")==0)
	{
		std::string email=extractEmail(req);
		//cout << "Client from <"+email+">\n";
		log_->debug("Client from <"+email+">");
		frm->second.from=email;
		sendStr(from,"250 Ok!\n");
	} else

	if (req.find("RCPT TO:")==0)
	{
		std::string email=extractEmail(req);
		//cout << "Client to <"+email+">\n";
		log_->debug("Client to <"+email+">");
		frm->second.to=email;
		sendStr(from,"250 Ok!\n");
	} else

	if (req.find("DATA")==0)
	{
		//cout << "Client sending data\n";
		log_->debug("Client sending data");
		frm->second.accepting=true;
		sendStr(from,"354 Ok!\n");
	} else

	if (req.find("QUIT")==0)
	{
		//cout << "Client quit\n\n";
		disconnectClient(from);
	}
}

//------------------------------------------------------------------------------------------
void Server::sendStr(int to, const std::string& str)
{
	send(to, str.c_str(), str.length(), 0);
}

//------------------------------------------------------------------------------------------
void Server::clientConnected(int client)
{
	log_->debug("Client connected...");
	fcntl(client, F_SETFL, O_NONBLOCK);
	//std::string answ="220 Ready to serve\n";
	//send(client, answ.c_str(), answ.length(), 0);
	sendStr(client, "220 Ready to serve\n");
	//Letter letter(accounts_,log_);
	//letter.accepting==false;
	//clients_[client]=letter;
	
	clients_.insert( std::map< int, Letter >::value_type ( client, Letter(accounts_,log_) ) );
	
	//log_->debug("Clients: " + itoa(clients_.size()));
	//log_->debug("ClientID: " + itoa(client));
}

//------------------------------------------------------------------------------------------
void Server::disconnectClient(int client)
{
	close(client);
	clients_.erase(client);
	log_->debug("Client disconnected...");
}

