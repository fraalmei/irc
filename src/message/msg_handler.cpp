/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:39:07 by p                 #+#    #+#             */
/*   Updated: 2025/10/09 13:44:59 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"
#include "../include/validNames.hpp"

msg_handler::msg_handler() {}

msg_handler::~msg_handler() {}

template<>
std::string		msg_handler::handle_buffer<std::string>(char* buffer, User *user, Server *Server)
{
	(void) buffer;
	(void) user;
	(void) Server;
	std::cout << "Handling buffer, return string." << std::endl;
	return "";
}

template<>
int				msg_handler::handle_buffer<int>(char* buffer, User *user, Server *Server)
{
	std::cout << "Se ha recibido un mensaje." << std::endl;
	std::cout << "Usuario: " << user->getFd() << std::endl;
	std::cout << "Buffer: " << buffer << "." << std::endl;

	if (user->getNickname().empty())
	{
		// Esperar mensaje tipo: NICK <nickname>
		std::string message(buffer);
		if (message.find("NICK ") == 0) {
			std::string nickname = message.substr(5);
			nickname.erase(nickname.find_last_not_of(WS) + 1);
			user->setNickname(nickname);
			std::string response = "Nickname registrado como " + nickname + "\n";
			send(user->getFd(), response.c_str(), response.size(), 0);
		} else {
			std::string response = "Por favor, envía tu nickname con: NICK <nickname>\n";
			send(user->getFd(), response.c_str(), response.size(), 0);
		}
		return 1; // No procesar más hasta tener nickname
	}

	// handle the buffer
	// tratar el buffer
	//msg_handler::handle_buffer<int>(buffer);
	//msg_andler::andle_buffer<std::string>(buffer);
	std::string message(buffer);
	if(message.find("JOIN ") == 0)
	{
		std::string channelName = message.substr(4);
		channelName.erase(0, channelName.find_first_not_of(WS));
		channelName.erase(channelName.find_last_not_of(WS) + 1);
		if (channelName.find_first_of(CHNAMEINVALID) != std::string::npos || (channelName[0] != REGCH && channelName[0] != LOCCH))
		{
			std::string response = "Channel name not valid. Must not contain whitespace and comma and must start with `#` or `&`.\n";
			send(user->getFd(), response.c_str(), response.size(), 0);
		}
		else
			Server->joinChannel(channelName, user);
	}

	if(message.find("show") == 0)
	{
		std::cout << "Canales" << std::endl;
		for (std::map<std::string, OChannel*>::const_iterator it = Server->getConstChannelList().begin(); it != Server->getConstChannelList().end(); ++it)
		{
			int i = it->second->getMembers().size();
			std::cout << it->first << " - " << i << std::endl;
		}
	}
	std::cout << "Handling buffer, return int." << std::endl;
	return 0;
}
