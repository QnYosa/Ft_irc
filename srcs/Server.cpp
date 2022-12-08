#include "Server.hpp"

Server::Server() : _servinfo(NULL), _server_socket_fd(0)
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

void	Server::printClients()
{
	std::map<const int, Client>::iterator it;
	for (it = _tmpClients.begin(); it != _tmpClients.end(); it++)
	{
		it->second.printClient();
	}
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

int	Server::addClientToTmp(int const &client_fd, char *message)
{
	std::vector<std::string> lines;
	this->split(lines, message);

	// return si nick deja present
	// si le fd et deja dans le map on le add sinon on fill le client correspondant.
	Client	client(client_fd);
	for (unsigned long i = 0; i != lines.size(); i++)
	{
		std::map<const int, Client>::iterator it;
		// std::cout  << RED << "line = " << RESET << lines[i] << std::endl; // on recupere bien chaque lignes
		std::cout << "client fd = " << client_fd << std::endl;
		it = _tmpClients.find(client_fd);
		if (it == _tmpClients.end())
		{
			fillClient(lines[i], client);
			_tmpClients.insert(std::pair<int, Client>(client_fd, client));
		}
		else
			fillClient(lines[i], it->second);
	}

	std::map<const int, Client>::iterator it1;
	it1 = _tmpClients.find(client_fd);
	if (this->confirmConnection(it1->second) == FAILURE)
	{
		// close(client_fd);
		_tmpClients.erase(it1);
		return (FAILURE);
	}
	return (SUCCESS);
}

