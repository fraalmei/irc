/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:39:07 by p                 #+#    #+#             */
/*   Updated: 2025/12/12 14:22:51 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"
#include "../include/User.hpp"
#include "../include/validNames.hpp"
#include "../include/colors.hpp"

int msg_handler::handle_buffer(std::string buffer, User *user)
{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " buffer -> " << buffer << std::endl;

	int msg_result = user->addToBuffer(buffer);
	if (msg_result == -1)
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Se ha recibido un mensaje cortado sin fin de línea." << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario: " << user->getFd() << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer: " << user->getBuffer() << "." << std::endl;
		return 1;
	}
	else if (msg_result == 0)
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Se ha recibido un mensaje vacio." << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario: " << user->getFd() << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer: " << user->getBuffer() << "." << std::endl;
		return 1;
	}
	else
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Se ha recibido un mensaje y está completo para parsear." << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario: " << user->getFd() << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer: " << user->getBuffer() << "." << std::endl;
	}
	return 0;
}

t_command msg_handler::parse_msg(std::string msg)
{
	t_command	command;
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << msg << std::endl;
	return command;
}
/*
	if(message.find("KICK ") == 0)
	{
		
	}
	if(message.find("JOIN ") == 0)
	{
		channelName = message.substr(4);
		channelName.erase(0, channelName.find_first_not_of(WS));
		channelName.erase(channelName.find_last_not_of(WS) + 1);
		if (channelName.find_first_of(CHNAMEINVALID) != std::string::npos || (channelName[0] != REGCH && channelName[0] != LOCCH))
		{
			response = "Channel name not valid. Must not contain whitespace and comma and must start with `#` or `&`.\n";
			send(user->getFd(), response.c_str(), response.size(), 0);
		}
		else
			Server->joinChannel(channelName, user);
	}

	if(message.find("show") == 0)
	{
		std::cout << "Canales" << std::endl;
		for (std::map<std::string, Channel*>::const_iterator it = Server->getConstChannelList().begin(); it != Server->getConstChannelList().end(); ++it)
		{
			int i = it->second->getMembers().size();
			std::cout << it->first << " - " << i << std::endl;
		}
	}

}

*/