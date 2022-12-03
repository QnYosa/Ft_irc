#include "Client.hpp"

// Client::Client(pollfd client_poll_fd) : _client_poll_fd(client_poll_fd)
// {
// 	std::cout << YELLOW << "Client constructor" << RESET << std::endl;
// }

Client::Client(int client_fd) : _client_fd(client_fd)
{
	std::cout << YELLOW << "Client constructor" << RESET << std::endl;
}

Client::~Client()
{
	std::cout << YELLOW << "Client destructor" << RESET << std::endl;
}

void	Client::setNickname(std::string const &nickname)
{
	_nickname = nickname;
	// limiter la size comme sur un irc classique
}

std::string		Client::getNickname()const
{
	return (_nickname);
}


void	Client::setUsername(std::string const &username)
{
	_fullname = username;
}

std::string 	Client::getUsername()const
{
	return (_fullname);
}

void			Client::printClient()const
{
	std::cout << RED << "nickname: " << _nickname << "\n" \
			  << "Fullname: " << _fullname << RESET << "\n";
}