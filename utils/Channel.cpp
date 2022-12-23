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
	removeOperator(clientName);
}

void	Channel::printClientList()
{
	std::cout << YELLOW << this->getName() << RESET << std::endl;
	for (std::map <std::string, Client>::iterator it = _clientList.begin(); it != _clientList.end(); it++)
		std::cout << it->first << std::endl;
}

void	Channel::addFirstOperator(std::string operatorName)
{
	if (_operators.empty())
		_operators.push_back(operatorName);
}

void	Channel::removeOperator(std::string operatorName)
{
	std::vector<std::string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == operatorName)
			_operators.erase(it);
	}
}

void	Channel::addOperator(std::string operatorName)
{
	// _operators.push_back(operatorName);
}

int 	Channel::isOperator(std::string &operatorName)
{
	std::vector<std::string>::iterator it;
	if (_operators.empty())
		return (FAILURE);
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == operatorName)
		{
			std::cout << "Hello\n";
			return (SUCCESS);
		}
	}
	return (FAILURE);
}

void	Channel::printOperators()
{
	std::cout << "Print operators\n";
	std::vector<std::string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}