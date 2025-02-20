#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Irc.hpp"

class Client
{
	private:
		// pollfd	_client_poll_fd;
		int			_client_fd;
		std::string	_nickname;
		std::string	_fullname;
		std::string	_realname;
		bool		_ready;
		// int			_operator;
	public:
		Client(int client_fd);
		~Client();
		void		setNickname(std::string const &nickname);
		std::string	& getNickname();
		void		setUsername(std::string const &username);
		std::string	getUsername()const;
		void		setRealname(std::string const &realname);
		std::string	getRealname()const;
		void		printClient()const;
		int			is_valid()const;
		// void		setOperator(int n);
		// int			getOperator()const;
};

#endif