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
		void		setNickname(std::string const &nickname);
		std::string	getNickname()const;
		void		setUsername(std::string const &username);
		std::string	getUsername()const;
		void		printClient()const;
};

#endif