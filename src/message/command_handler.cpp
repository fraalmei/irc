/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 16:37:21 by p                 #+#    #+#             */
/*   Updated: 2026/01/17 18:00:38 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"


/// @brief join or create a channel
///			entrar o crear un canal
/// @param channelName
/// @param new_client
/// @param server
int				msg_handler::joinChannel(const std::string channelName, User *new_client, Server &server)
{
	Channel* channel = server.getChannelByName(channelName);

	std::string msg;
	if (channel == NULL)
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " "<< channelName << " not found" << std::endl;
		// Crear nuevo canal público
		Channel* newChannel = new Channel(channelName);
		newChannel->addMember(new_client);	// primer miembro
		server.getChannelList()[channelName] = newChannel;
		msg = "JOIN " + newChannel->getName() + "\r\n";
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Created channel: " << channelName << std::endl;
	}
	else
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Found channel: " << channel->getName() << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " " << channel->isMember(new_client->getFd()) << std::endl;
		if (!channel->isMember(new_client->getFd()))
		{
			if (channel->addMember(new_client) != 0 ) // Agregar al canal existente
				std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error: User already in channel " << channelName << std::endl;
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " User " << new_client->getFd() << " joined " << channelName << std::endl;
			msg = "JOIN " + channel->getName() + "\r\n";
		}
		else
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error: User already in channel " << channelName << std::endl;
			msg = new_client->getUsername() + " " + new_client->getUsername() + " " + channelName + " :is already on channel\r\n";
			send(new_client->getFd(), msg.c_str(), msg.length(), 0);
			return 1;
		}
	}

	// Mensaje de confirmación
	return 0;
}

