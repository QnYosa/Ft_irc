#include "Channel.hpp"

Channel::Channel(std::string const &channelName): _name(channelName)
{
	// std::cout << "Channel constructor\n";
}

Channel::~Channel()
{
	// std::cout << "Channel destructor" << getName() << std::endl;
}

std::string										Channel::getName()const
{
	return (_name);
}

std::map <std::string, Client>	Channel::getClientList()const
{
	return (_clientList);
}

int		Channel::findClient(std::string &clientName)
{
	std::map <std::string, Client>::iterator it = _clientList.find(clientName);
	if (it == _clientList.end())
		return (FAILURE);
	return (SUCCESS);
}

void	Channel::addClientToChannel(Client &client)
{
	this->_clientList.insert(std::pair<std::string, Client>(client.getNickname(), client));
}

void	Channel::removeClientFromChannel(std::string &clientName)
{
	//find et remove .
	std::map <std::string, Client>::iterator it = this->_clientList.find(clientName);
	if (it != _clientList.end())
		this->_clientList.erase(it);
}

void	Channel::printClientList()
{
	std::cout << YELLOW << this->getName() << RESET << std::endl;
	for (std::map <std::string, Client>::iterator it = _clientList.begin(); it != _clientList.end(); it++)
		std::cout << it->first << std::endl;
}