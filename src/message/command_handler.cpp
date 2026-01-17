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
		channel = newChannel;
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
		}
		else
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error: User already in channel " << channelName << std::endl;
			std::stringstream ss;
			ss << ERR_NOTONCHANNEL;
			std::string err_code = ss.str();
			std::string err = ":" + std::string(ME) + " " + err_code + " " + new_client->getNickname() + " " + channelName + " :You're not on that channel\r\n";
			send(new_client->getFd(), err.c_str(), err.length(), 0);
			return 1;
		}
	}

	// Send JOIN to all members
	std::string join_msg = ":" + new_client->getNickname() + "!" + new_client->getUsername() + "@localhost JOIN " + channel->getName() + "\r\n";
	for (std::vector<User*>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it)
	{
		send((*it)->getFd(), join_msg.c_str(), join_msg.length(), 0);
	}

	// Send RPL_NAMREPLY and RPL_ENDOFNAMES to the new client
	std::string names_list;
	for (std::vector<User*>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it)
	{
		if (!names_list.empty()) names_list += " ";
		names_list += (*it)->getNickname();
	}
	std::stringstream ss;
	ss << RPL_NAMREPLY;
	std::string nam_code = ss.str();
	ss.str(""); ss << RPL_ENDOFNAMES;
	std::string end_code = ss.str();
	
	std::string namreply = ":" + std::string(ME) + " " + nam_code + " " + new_client->getNickname() + " = " + channel->getName() + " :" + names_list + "\r\n";
	send(new_client->getFd(), namreply.c_str(), namreply.length(), 0);

	std::string endofnames = ":" + std::string(ME) + " " + end_code + " " + new_client->getNickname() + " " + channel->getName() + " :End of /NAMES list\r\n";
	send(new_client->getFd(), endofnames.c_str(), endofnames.length(), 0);

	return 0;
}

