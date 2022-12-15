#include "Irc.hpp"
#include "Server.hpp"

void	parsing(std::string action)
{
	for (int i = 0; i < action.length(); i++)
		action[i] = tolower(action[i]);
	if (action == "join")
	{
		std::cout << "JOIN\n";
	}
	else
		std::cout << "action not found\n";
}



int main (void)
{
	Server s;

	// Channel channel(channel_name);
	// channel.getClientList().insert(channel_name, std::vector <Client>);
	// s._channels.insert(std::pair<std::string, Channel>(channel_name, channel));
	// Client client(4);
	// parsing("join");
	s.addChannel("Ligue 1");
	s.addChannel("ligue 2");
	s.addChannel("Nationale");
	s.addChannel("Serie A");
	std::map<std::string, Channel> tmp_channel = s.getChannels();
	std::cout << "size = " << tmp_channel.size() << std::endl;
	std::map<std::string, Channel>::iterator it = tmp_channel.begin();
	std::cout << "begin  = " << it->second.getName() << std::endl;
	it = s.getChannels().end();
	std::cout << "end  = " << it->second.getName() << std::endl;
	for (it = tmp_channel.begin(); it != tmp_channel.end(); it++)
	{
		std::cout << "Channels = " << it->second.getName() << std::endl;
	}
	return (0);
}