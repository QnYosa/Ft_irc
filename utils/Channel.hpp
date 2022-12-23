#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Irc.hpp"
#include "Client.hpp"

class	Client;

class Channel
{
	private:
		std::map <std::string, Client>	_clientList;
		std::vector<std::string>		_kicked;
		std::vector<std::string>		_banned;
		std::vector<std::string>		_operators;
		std::string 					_name;
	public:
		Channel(std::string const &name);
		~Channel();
		std::string						getName()const;
		std::map <std::string, Client>	getClientList()const;
		int								findClient(std::string &clientName);
		void							addClientToChannel(Client &client);
		void							printClientList();
		void							removeClientFromChannel(std::string &clientName);
		/*								OPERATORS					*/
		void							addFirstOperator(std::string operatorName);
		void							removeOperator(std::string operatoName);
		void							addOperator(std::string operatorName);
		int								isOperator(std::string &operatorName);
		void							printOperators();
};

#endif