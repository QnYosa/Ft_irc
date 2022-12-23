#include "Server.hpp"

void	Server::addChannel(std::string const &channelName)
{
	// check if channel already exist.
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	if (it != _channels.end())
	{
		std::cout << "Channel already exists, choose an other name\n";
		return ;
	}
	Channel	channel(channelName);
	_channels.insert(std::pair<std::string, Channel>(channelName, channel));
	// std::cout << RED << "Channel added: " << channelName << RESET << std::endl;
}

void	Server::addClientToChannel(std::string &channelName, Client &client)
{
	std::map<std::string, Channel>::iterator it;
	it = _channels.find(channelName);
	if (it->second.findClient(client.getNickname()) == FAILURE)
	{
		it->second.addClientToChannel(client);
	}
	else 
		std::cout << YELLOW << client.getNickname() << "already here\n" << RESET;
}	

void	Server::join(Client &client, std::string &channelName)
{
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	if (it == _channels.end())
		this->addChannel(channelName);
	addClientToChannel(channelName, client);
	it->second.addFirstOperator(client.getNickname());
	// add client to operators if empty.
}

void	Server::printChannel(std::string &channelName)
{
	std::map<std::string, Channel> tmp;
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	it->second.printClientList();
}

void	Server::quit(std::string &channelName, std::string &clientName)
{
	std::map<std::string, Channel>::iterator it;
	it = _channels.find(channelName);
	if (it->second.findClient(clientName) == SUCCESS)
	{
		it->second.removeClientFromChannel(clientName);
		std::cout << clientName << " has now left " << channelName << std::endl; 
	}
}

void	Server::kick(std::string &operatorName, std::string &channelName, std::string &clientName)
{
	// if (client.getOperator() == 0)
	// {
	// 	std::cout << "You're not allowed to kick someone\n";
	// 	return ;
	// }
	std::map<std::string, Channel>::iterator it;
	it = _channels.find(channelName);
	if (it->second.findClient(clientName) == SUCCESS)
	{
		if (it->second.isOperator(operatorName) == FAILURE)
		{
			std::cout << operatorName << " is not admin on " << channelName << std::endl;
			return ;
		}
		it->second.removeClientFromChannel(clientName);
		std::cout << clientName << " has been kicked from " << channelName << " by " << operatorName <<std::endl; 
	}	
}

void	Server::oper(std::string channelName, std::string operatorName, std::string password)
{
	if (password != _operatorPassword)
	{
		std::cout << "Wrong Password\n";
		return ;
	}
	std::map<std::string, Channel>::iterator it;
	it = _channels.find(channelName);
	if (it->second.isOperator(operatorName) == FAILURE)
		it->second.addOperator(operatorName);
}