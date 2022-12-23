#include "Irc.hpp"
#include "Server.hpp"

int main (void)
{
	Server s;

	// Channel channel(channel_name);
	// channel.getClientList().insert(channel_name, std::vector <Client>);
	// s._channels.insert(std::pair<std::string, Channel>(channel_name, channel));
	Client client(4);
	client.setNickname("Bassim");
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
	// for (it = tmp_channel.begin(); it != tmp_channel.end(); it++)
	// {
	// 	std::cout << "Channels = " << it->second.getName() << std::endl;
	// }
	std::string l1 = "Ligue 1";
	std::string l2 = "Rugby";
	Client c2(4);
	std::string errorstr = "jeff";
	c2.setNickname("Dimitri");
	// c2.setOperator(1);
	s.join(c2, l1);
	s.join(client, l1);
	s.kick(c2.getNickname(), l1, client.getNickname());
	s.printChannel(l1);
	// s.quit(l1, c2.getNickname());
	s.quit(l1, errorstr);
	s.printChannel(l1);
	std::cout << "Hello\n";
	s.join(client, l2);
	s.printChannel(l2);
	s.join(c2, l1);
	s.oper("Ligue1", client.getNickname(), "pswd");
	s.oper("Ligue 1", client.getNickname(), "pswd");
	s.printOper(l1);
	return (0);
}