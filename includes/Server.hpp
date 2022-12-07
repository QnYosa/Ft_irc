#ifndef SERVER_HPP
#define	SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"

class Server
{
	private:
		struct addrinfo				_hints;
		struct addrinfo				*_servinfo;
		std::map<const int, Client>	_clients;
		std::map<const int, Client>	_tmpClients;
		int							_server_socket_fd;
	public:
		Server();
		~Server();
		void		setHints();
		int			fillServinfo(char *port);
		void		printClients();
		int			launchServer();
		int			manageServerLoop();
		void		fillClient(std::string line, Client &client); // needs ameliorations.
		int			addClientToTmp(int const &client_fd, char *message); // works 
		int			confirmConnection(Client const &client);
};

#endif