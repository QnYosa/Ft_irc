#ifndef SERVER_HPP
#define	SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"

class	Client;
class	Channel;

class Server
{
	private:
		struct addrinfo					_hints;
		struct addrinfo					*_servinfo;
		std::map<const int, Client>		_clients;
		std::map<const int, Client>		_tmpClients;
		int								_server_socket_fd;
		std::map<std::string, Channel> _channels;
	public:
		Server();
		~Server();
		void		setHints();
		int			fillServinfo(char *port);
		int			launchServer();
		int			manageServerLoop();
		void		fillClient(std::string line, Client &client); // needs ameliorations.
		int			addClientToTmp(int const &client_fd, char *message); // works 
		int			confirmConnection(Client const &client, const int client_fd);
		std::map<std::string, Channel>	getChannels()const;
		void		addChannel(std::string const &channelName);
		void		addClientToChannel(std::string &channelName, Client &client);

};

#endif