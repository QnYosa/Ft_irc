#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Irc.hpp"
#include "Client.hpp"

class	Client;

class Channel
{
	private:
		std::map <std::string, Client> _clientList;
		std::string _name;
	public:
		Channel(std::string const &name);
		~Channel();
		std::string						getName()const;
		std::map <std::string, Client>	getClientList()const;
		void							addClientToChannel(Client &client);
};

#endif