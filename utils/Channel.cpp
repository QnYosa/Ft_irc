#include "Channel.hpp"

Channel::Channel(std::string const &channelName): _name(channelName)
{
	// std::cout << "Channel constructor\n";
}

Channel::~Channel()
{
	std::cout << "Channel destructor" << getName() << std::endl;
}

std::string										Channel::getName()const
{
	return (_name);
}

std::map <std::string, Client>	Channel::getClientList()const
{
	return (_clientList);
}

void	Channel::addClientToChannel(Client &client)
{
	_clientList.insert(std::pair<std::string, Client>(client.getNickname(), client));
}
