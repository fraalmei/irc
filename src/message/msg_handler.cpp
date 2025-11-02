/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraalmei <fraalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:39:07 by p                 #+#    #+#             */
/*   Updated: 2025/11/02 15:37:44 by fraalmei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"
#include "../include/validNames.hpp"

msg_handler::msg_handler() {}

msg_handler::~msg_handler() {}


int				msg_handler::handle_buffer(char* buffer, User *user, Server *Server)
{
	std::string response;

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
			response = "Nickname registrado como " + nickname + "\n";
		} else {
			response = "Por favor, envía tu nickname con: NICK <nickname>\n";
		}
		send(user->getFd(), response.c_str(), response.size(), 0);
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
			response = "Channel name not valid. Must not contain whitespace and comma and must start with `#` or `&`.\n";
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

int				msg_handler::handle_nickname(char* buffer, User *user)
{
	std::cout << "Dentro de handle_nickname." << std::endl;
	if (buffer == NULL || user == NULL || buffer[0] == '\0')
		return 1;
	user->setNickname(std::string(buffer));
	std::cout << "Handling nickname, return int." << std::endl;
	return 0;
}

int				msg_handler::handle_username(char* buffer, User *user)
{
	std::cout << "Dentro de handle_username." << std::endl;
	if (buffer == NULL || user == NULL || buffer[0] == '\0')
		return 1;
	user->setUsername(std::string(buffer));
	std::cout << "Handling username, return int." << std::endl;
	return 0;
}

int				msg_handler::handle_password(char* buffer, User *user, Server *Server)
{
	std::cout << "Dentro de handle_password." << std::endl;
	if (buffer == NULL || user == NULL || buffer[0] == '\0')
		return 1;
	if (Server->get_password() != buffer)
	{
		std::cout << "Invalid password from user " << user->getFd() << "." << std::endl;
		std::cout << "Server password " << Server->get_password() << "." << std::endl;
		std::cout << "User password " << buffer << "." << std::endl;
		return 2;
	}
	user->setAuthenticated(true);
	std::cout << "Handling password, return int." << std::endl;
	return 0;
}
