#include "Server.hpp"
#include "Colors.hpp"

static int	acceptSocket(int listenSocket)
{
	sockaddr_in	client;
	socklen_t	addr_size = sizeof(sockaddr_in);
	return (accept(listenSocket, (sockaddr *)&client, &addr_size));
}

// Should be a member function of Server Class
static void	addClient(int client_socket, std::vector<pollfd> &poll_fds, std::map<const int, Client>	&clients_list)
{
	pollfd	client_pollfd;
	Client	client(client_socket);

	client_pollfd.fd = client_socket;
	client_pollfd.events = POLLIN;
	poll_fds.push_back(client_pollfd);
	
	clients_list.insert(std::pair<int, Client>(client_socket, client)); // insert a new nod in client map with the fd as key
	
	std::cout << PURPLE << "ADDED CLIENT SUCCESSFULLY" << RESET << std::endl;
}

static void	tooManyClients(int client_socket)
{
	std::cout << RED << ERR_FULL_SERV << RESET << std::endl;
	send(client_socket, ERR_FULL_SERV, strlen(ERR_FULL_SERV) + 1, 0);
	close(client_socket);
}

static sockaddr_in	getClientAddress(int socket)
{
	sockaddr_in client;
	socklen_t addrSize = sizeof(struct sockaddr_in);
	if (getpeername(socket, (struct sockaddr*)&client, &addrSize) != SUCCESS)
	{
		std::cerr << RED << "Get Client Address Error" << RESET << std::endl;
		throw ;
	}
	return (client);
}

static void	print(std::string type, int client_socket, char *message)
{
	sockaddr_in client = getClientAddress(client_socket);
	std::cout << PURPLE << type \
			  << RESET << client_socket << " " \
			  << inet_ntoa(client.sin_addr) << " " \
			  << ntohs(client.sin_port) << std::endl \
			  << BLUE << (message == NULL ? "\n" : message) << RESET << std::endl;
}

// Should be a member function of Server Class
static void	delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator &it, std::map<const int, Client>	&clients_list)
{
	std::cout << "je suis dans le del\n";
	print("Deconnection of client : ", it->fd, NULL);
	std::vector<pollfd>::iterator		iterator;
	for (iterator = poll_fds.begin(); iterator != poll_fds.end(); iterator++)
	{
		if (iterator->fd == it->fd)
		{
			close(it->fd);
			poll_fds.erase(iterator);
			clients_list.erase(it->fd);
			break;
		}
	}
	std::cout << CYAN << "Client deleted \nTotal Client is now: " << (unsigned int)(poll_fds.size() - 1) << RESET << std::endl;
}

int		Server::manageServerLoop()
{
	std::vector<pollfd>	poll_fds;
	pollfd				server_poll_fd;

	server_poll_fd.fd = _server_socket_fd;
	server_poll_fd.events = POLLIN;

	poll_fds.push_back(server_poll_fd);

	while (1)
	{
		std::vector<pollfd> new_pollfds; // tmp struct hosting potential newly-created fds

		if (poll((pollfd *)&poll_fds[0], (unsigned int)poll_fds.size(), -1) <= SUCCESS) // -1 == no timeout
		{
			std::cerr << RED << "Poll error" << RESET << std::endl;;
			return (FAILURE);
		}

		std::vector<pollfd>::iterator	it = poll_fds.begin();
		while (it != poll_fds.end())
		{
			if (it->revents & POLLIN) // => If the event that occured is a POLLIN (aka "data is ready to recv() on this socket")
			{
				if (it->fd == _server_socket_fd)
				{
					int	client_sock = acceptSocket(_server_socket_fd); // Accepts the socket and returns a dedicated fd for this new Client-Server connexion
					if (client_sock == -1)
					{
						std::cerr << RED << "Accept failed" << RESET << std::endl;
						continue;
					}
					if (poll_fds.size() - 1 < MAX_CLIENT_NB)
						addClient(client_sock, new_pollfds, _clients); // Beware, here we push the new client_socket in NEW_pollfds
					else
						tooManyClients(client_sock);
					it++;
				}
				else // => If the dedicated fd for the Client/Server connection already exists
				{
					char	message[BUF_SIZE_MSG];
					int		read_count;

					memset(message, 0, sizeof(message));
					read_count = recv(it->fd, message, BUF_SIZE_MSG, 0); // Retrieves the Client's message

					if (read_count <= FAILURE) // when recv returns an error
					{
						std::cerr << RED << "Recv() failed [456]" << RESET << std::endl;
						delClient(poll_fds, it, _clients);
					}
					else if (read_count == 0) // when a client disconnects
					{
						delClient(poll_fds, it, _clients);
						std::cout << "Disconnected\n";
					}
					else
					{
						print("Recv : ", it->fd, message); // si affichage incoherent regarder ici 
						// parsing
						// TODO : récup la fonction fillClient de Dim et la décomposer :
						// TODO : - split le 
						// TODO : - fill le client qui a deja été add avec les infos du message
						// TODO : - check en plus du mdp
						// TODO : Normalement le premier message du client fini par USER... 
						// TODO : donc le check si le client est bon peut se faire à ce moment la car on est censé avoir recu le MDP et le NICK avant
						// send(it->fd, ":127.0.0.1 001 tmanolis :Welcome tmanolis!tmanolis@127.0.0.1\r\n", 62, 0);
						// print("Send : ", it->fd, message);
						it++;
					}
				}
			}
			else if (it->revents & POLLERR) // voir si il faut it++ ?
			{
				std::cout << "je suis dans le POLLERR\n";
				if (it->fd == _server_socket_fd)
				{
					std::cerr << RED << "Listen socket error" << RESET << std::endl;
					return (FAILURE);
				}
				else
				{
					std::cout << "dans le else\n";
					delClient(poll_fds, it, _clients);
				}
			}
			else
				it++;
		}
		poll_fds.insert(poll_fds.end(), new_pollfds.begin(), new_pollfds.end()); // Add the range of NEW_pollfds in poll_fds (helps recalculating poll_fds.end() in the for loop)
		std::cout << "j'ai insert\n" << std::endl;
		// print list of our client
		std::map<const int, Client>::iterator it_map;
		for (it_map = _clients.begin(); it_map != _clients.end(); it_map++)
		{
			it_map->second.printClient();
		}
	}
	return (SUCCESS);
}