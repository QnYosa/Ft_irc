#include "Server.hpp"

Server::Server() : _servinfo(NULL), _server_socket_fd(0), _operatorPassword("pswd")
{
	std::cout << YELLOW << "Server Constructor" << RESET << std::endl;
	memset(&_hints, 0, sizeof(_hints));
}

Server::~Server()
{
	std::cout << YELLOW << "Server destructor" << RESET << std::endl;
}

/**
 * @brief Attributes the correct parameters to the structure Hints.
 * 
 */
void	Server::setHints()
{
	_hints.ai_family = AF_INET; // We choose Ipv4
	_hints.ai_socktype = SOCK_STREAM; // We choose to work with TCP stream sockets
	_hints.ai_flags = AI_PASSIVE; // We'll be on localhost by default
}

/**
 * @brief Helps set up the structs 'hints' and 'servinfo' of our Server class
 * 
 * @param port Value given by the user (e.g. "6667")
 * @return int Returns SUCCESS or FAILURE whether getaddrinfo works or not
 */
int		Server::fillServinfo(char *port)
{
	if (getaddrinfo(NULL, port, &_hints, &_servinfo) < 0)
	{
		std::cerr << RED << "Flop du addrinfo" << RESET << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);
}

/**
 * @brief This function follows step by step the required function calls to launch the server:
 * 
 * 		1) socket() => get the server socket file descriptor
 * 		2) setsocktop() => enable the configuration of said socket (here, we wanted
 * 							to allow the re-use of a port if the IP address is different)
 * 		3) bind() => Associate the socket with a specific port (here, the one given by the user)
 * 		4) listen() => Wait for any incoming connections to our server socket
 * 
 * @return int 0 for SUCCESS and -1 for FAILURE
 */
int		Server::launchServer()
{
	_server_socket_fd = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
	if (_server_socket_fd == FAILURE)
	{
		std::cerr << RED << "Flop de la socket :(" << RESET << std::endl;
		return (FAILURE);
	}
	int optvalue = 1; // enables the re-use of a port if the IP address is different
	if (setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue)) == FAILURE)
	{
		std::cerr << RED << "Impossible to reuse" << RESET << std::endl;
		return (FAILURE);
	}
	if (bind(_server_socket_fd, _servinfo->ai_addr, _servinfo->ai_addrlen) == FAILURE)
	{
		std::cerr << RED << "Bind failed" << RESET << std::endl;;
		return (FAILURE);
	}
	if (listen(_server_socket_fd, BACKLOG) == FAILURE)
	{
		std::cerr << RED << "Listen failed" << RESET << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);
}

void	Server::fillClient(std::string line, Client &client)
{
	if (line.find("NICK") != std::string::npos)
	{
		line.erase(line.find("NICK"), 4);
		client.setNickname(line);
	}
	else if (line.find("USER") != std::string::npos)
	{
		// faire une subst qui return le name, set le username supprimer jusqau prochain espace recuperer les infos
		// faire attentions aux valeurs de retour si un bail n'est pas envoye 
		// faire attention aux * 
		// encore une fois il faudra tester en changeant les parametres par defaut.
		// resplitter la line ? plus efficace sans doute. 
		line.erase(line.find("USER "), 5);
		client.setUsername(line.substr(line.find(" "), line.find(" ") + 1));
		client.setRealname(line.substr(line.find(":") + 1, line.length() - line.find(":") + 1));
		// limite a cause d'une succession d'espaces notamment dans le username.
		// tester si on met plusieurs espaces a la suite.
		// faire tests avec un username modifie
		// :realname
	}
	// else
	// 	std::cout << "no NICk line = " << line << std::endl;
}

int	Server::addClientToTmp(int const &client_fd, char *message)
{
	std::string msg = message;
	std::string	delimiter = "\n";
	int pos = 0;
	std::string	substr;
	std::vector<std::string> lines;

	int i = 0;
	while ((pos = msg.find(delimiter)) != static_cast<int>(std::string::npos))
	{
		substr = msg.substr(0, pos);
		// std::cout << "line = " << RED << substr << RESET << std::endl;
		std::cout << "i = " << RED << i << RESET << std::endl;
		lines.push_back(substr);
		msg.erase(0, pos + delimiter.length());
		i++;
	}
	// si le fd et deja dans le map on le add sinon on fill le client correspondant.
	Client	client(client_fd);
	for (unsigned long i = 0; i != lines.size(); i++)
	{
		std::map<const int, Client>::iterator it;
		it = _tmpClients.find(client_fd);
		if (it == _tmpClients.end())
		{
			fillClient(lines[i], client);
			_tmpClients.insert(std::pair<int, Client>(client_fd, client));
		}
		else
		{
			fillClient(lines[i], it->second);
		}
	}
	// return (client ready());
	return (this->confirmConnection(_tmpClients.find(client_fd)->second, client_fd));
}

int	Server::confirmConnection(Client &client, const int client_fd)
{
	if (client.is_valid() == FAILURE)
		return (461); //  ERR_NEEDMOREPARAMS (461)
	std::map<const int, Client>::iterator it;
	for (it = _tmpClients.begin(); it != _tmpClients.end(); it++)
	{
		if (it->first != client_fd && it->second.getNickname() == client.getNickname())
		{
			close(client_fd);
			_tmpClients.erase(it);
			return (433); // 433 ERR_NICKNAMEINUSE
		}
	}
	std::string	message;
	std::string wlcm = RPL_WELCOME;
	std::string wlcm_msg = WELCOME_MSG;
	std::string spc = " ";
	// return_value = clientValid();
	message.append("localhost");
	message.append(spc);
	message.append(wlcm);
	message.append(client.getNickname());
	message.append(" ");
	// message.append("Welcome on our very first Irc Server");
	// std::cout << "message 1 = " << message << std::endl;
	// std::cout << "message 2 = " << message << std::endl;
	return (SUCCESS);
}

std::map<std::string, Channel>	Server::getChannels()const
{
	return (_channels);
}



