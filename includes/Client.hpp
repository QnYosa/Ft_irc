#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Irc.hpp"

class Client
{
	private:
		// pollfd	_client_poll_fd;
		int		_client_fd;
		std::string	_nickname;
		std::string	_fullname;
	public:
		Client(int client_fd);
		~Client();
};

#endif