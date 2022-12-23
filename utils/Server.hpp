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
		std::map<std::string, Channel>	_channels;
		std::string						_operatorPassword;
	public:
		Server();
		~Server();
		void		setHints();
		int			fillServinfo(char *port);
		int			launchServer();
		int			manageServerLoop();
		void		fillClient(std::string line, Client &client); // needs ameliorations.
		int			addClientToTmp(int const &client_fd, char *message); // works 
		int			confirmConnection(Client &client, const int client_fd);
		std::map<std::string, Channel>	getChannels()const;
		void		addChannel(std::string const &channelName);
		void		addClientToChannel(std::string &channelName, Client &client);
	/* Server Commands */
		void		join(Client &client, std::string &channelName);
		void		quit(std::string &channelName, std::string &clientName);
		void		kick(std::string &operatorName, std::string &channelName, std::string &clientName);
		void		ban(std::string &operatorName, std::string &channelName, std::string &clientName);
		void		oper(std::string channelName, std::string operatorName, std::string password);
	/*Display*/
		void		printChannel(std::string &channelName);
		void		printOper(std::string &channelName);
};

#endif