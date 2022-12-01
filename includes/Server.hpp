#ifndef SERVER_HPP
#define	SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"

class Server
{
	private:
		struct addrinfo			_hints;
		struct addrinfo			*_servinfo;
		std::map<int, Client>	_clients;
		std::map<int, Client>	_tmpClients;
		int						_server_socket_fd;
	public:
		Server();
		~Server();
		void	setHints();
		int		fillServinfo(char *port);
		int		launchServer();
		int		manageServerLoop();
		void	fillClient(std::string const &line);
		void	addClientToTmp(int const &client_fd, char *message);
};

#endif