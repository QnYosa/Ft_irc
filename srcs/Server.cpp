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

std::string	Server::getMdp() 
{ 
	return (_mdp);
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

void	Server::addClient(int client_socket, std::vector<pollfd> &poll_fds)
{
	pollfd	client_pollfd;
	Client	new_client(client_socket);

	client_pollfd.fd = client_socket;
	client_pollfd.events = POLLIN;
	poll_fds.push_back(client_pollfd);
	
	_clients.insert(std::pair<int, Client>(client_socket, new_client)); // insert a new nod in client map with the fd as key
	std::cout << PURPLE << "ADDED CLIENT SUCCESSFULLY" << RESET << std::endl;
	std::cout << "Map size : " << _clients.size() << std::endl;
	std::map<const int, Client>::iterator it_map;
	for (it_map = _clients.begin(); it_map != _clients.end(); it_map++)
	{
		std::cout << "Key : " << it_map->first << std::endl;
		it_map->second.printClient();
	}
}

void	Server::delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator &it)
{
	std::cout << "je suis dans le del\n";
	std::cout << "Deconnection of client : " << it->fd << std::endl;
	int key = it->fd;
	std::vector<pollfd>::iterator		iterator;
	for (iterator = poll_fds.begin(); iterator != poll_fds.end(); iterator++)
	{
		if (iterator->fd == it->fd)
		{
			close(it->fd);
			poll_fds.erase(iterator);
			_clients.erase(key);
			break;
		}
	}
	std::cout << CYAN << "Client deleted \nTotal Client is now: " << (unsigned int)(poll_fds.size() - 1) << RESET << std::endl;
	std::cout << "Map size : " << _clients.size() << std::endl;
	std::map<const int, Client>::iterator it_map;
	for (it_map = _clients.begin(); it_map != _clients.end(); it_map++)
	{
		std::cout << "Key : " << it_map->first << std::endl;
		it_map->second.printClient();
	}
}


