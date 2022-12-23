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
	if (it->second.isBanned(client.getNickname()) == SUCCESS)
	{
		std::cout << client.getNickname() << " is banned from " << channelName << std::endl; 
		return ;
	}
	addClientToChannel(channelName, client);
	it->second.addFirstOperator(client.getNickname());
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
	if (it == _channels.end())
	{
		std::cout << "Channel not found\n";
		return ;
	}
	if (it->second.findClient(clientName) == SUCCESS)
	{
		it->second.removeClientFromChannel(clientName);
		std::cout << clientName << " has now left " << channelName << std::endl; 
	}
}

void	Server::kick(std::string &operatorName, std::string &channelName, std::string &clientName)
{
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

void	Server::ban(std::string &operatorName, std::string &channelName, std::string &clientName)
{
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
		it->second.addToBanned(clientName);
		std::cout << clientName << " has been banned from " << channelName << " by " << operatorName <<std::endl; 
	}	
}

void	Server::unban(std::string &operatorName, std::string &channelName, std::string &unbanned_name)
{
	std::map<std::string, Channel>::iterator it;
	it = _channels.find(channelName);
	if (it->second.findClient(unbanned_name) == SUCCESS)
	{
		if (it->second.isOperator(operatorName) == FAILURE)
		{
			std::cout << operatorName << " is not admin on " << channelName << std::endl;
			return ;
		}
		it->second.addClientToChannel(unbanned_name);
		it->second.removeFromBanned(unbanned_name)
		std::cout << unbanned_name << " has been unbanned from " << channelName << " by " << operatorName <<std::endl; 
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
	if (it == _channels.end())
	{
		std::cout << "That channel doesn't exist\n";
		return ;
	}
	if (it->second.isOperator(operatorName) == FAILURE)
		it->second.addOperator(operatorName);
}

void	Server::printOper(std::string &channelName)
{
	std::map<std::string, Channel>::iterator it;
	it = _channels.find(channelName);
	if (it == _channels.end())
	{
		std::cout << "That channel doesn't exit\n";
		return ;
	}
	it->second.printOperators();
}