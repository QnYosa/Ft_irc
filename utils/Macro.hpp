#ifndef	MACRO_HPP
#define MACRO_HPP

#include "Irc.hpp"

#define SUCCESS 0
#define FAILURE -1

#define BACKLOG 10          // Nb max of clients queuing
#define MAX_CLIENT_NB 4     // Nb max of clients overall
#define BUF_SIZE_MSG 4096

/*		MESSAGE		*/
#define ERR_FULL_SERV "Server already full"
#define WELCOME_MSG "Welcome on our very first Irc Server"
#define RPL_WELCOME "001"
#define ERR_NICKNAMEINUSE "433"
#endif