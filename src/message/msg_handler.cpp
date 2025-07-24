/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:39:07 by p                 #+#    #+#             */
/*   Updated: 2025/07/24 13:53:31 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"

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
	if (user->getNickname().empty())
	{
		// Esperar mensaje tipo: NICK <nickname>
		std::string message(buffer);
		if (message.find("NICK ") == 0) {
			std::string nickname = message.substr(5);
			nickname.erase(nickname.find_last_not_of(" \n\r\t") + 1);
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
		std::string channelName = message.substr(6);
		channelName.erase(channelName.find_last_not_of(" \n\r\t") + 1);
		Server->joinChannel(channelName, user);
	}

	if(message.find("show") == 0)
	{
		std::cout << "Canales" << std::endl;
		for (std::map<std::string, AChannel*>::const_iterator it = Server->getConstChannelList().begin(); it != Server->getConstChannelList().end(); ++it)
		{
			int i = it->second->getMembers().size();
			std::cout << it->first << " - " << i << std::endl;
		}
	}
	std::cout << "Handling buffer, return int." << std::endl;
	return 0;
}
