#ifndef PARSE_HPP
#define PARSE_HPP

#include "Server.hpp"

void	Server::fillClient(std::string line, Client &client)
{
	if (line.find("NICK") != std::string::npos)
	{
		line.erase(line.find("NICK"), 4);
		client.setNickname(line);
	}
	else if (line.find("USER") != std::string::npos)
	{
		line.erase(line.find("USER "), 5);
		client.setUsername(line.substr(line.find(" "), line.find(" ") + 1));
		client.setRealname(line.substr(line.find(":") + 1, line.length() - line.find(":") + 1));
	}
}

void	Server::split(std::vector<std::string> &lines, char *message)
{
	std::string msg = message;
	std::string	delimiter = "\n";
	int pos = 0;
	std::string	substr;
	while ((pos = msg.find(delimiter)) != static_cast<int>(std::string::npos))
	{
		substr = msg.substr(0, pos);
		lines.push_back(substr);
		msg.erase(0, pos + delimiter.length());
	}
}

int	Server::confirmConnection(Client const &client)
{
	if (client.is_valid() == FAILURE)
		return (461);
	std::map<const int, Client>::iterator it;
	for (it = _tmpClients.begin(); it != _tmpClients.end(); it++)
	{
		if (it->second.getNickname() == client.getNickname() && it->first != client.getClientFd())
		{
			std::cout << "same nick\n";
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

#endif