/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_getset.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:31:13 by fraalmei          #+#    #+#             */
/*   Updated: 2026/01/30 10:04:41 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


User	*Server::getClientByFd(int fd)
{
	// Search for the client by its file descriptor
	// Buscar el cliente por su descriptor de archivo
	for(std::map<int, User*>::const_iterator it = getConstClientList().begin(); it != getConstClientList().end(); ++it)
		if (it->first == fd)
			return it->second; // Return the found client
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " User with fd " << fd << " not found." << std::endl;
	return NULL; // Exit if the client is not found
}

User	*Server::getClientFdByNickname(const std::string &nickname)
{
	// Search for the client by its nickname (member definition)
	// Buscar el cliente por su nickname
	for(std::map<int, User*>::const_iterator it = this->getConstClientList().begin(); it != this->getConstClientList().end(); ++it)
		if (it->second->getNickname() == nickname)
			return it->second; // Return the found client
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " User with nickname " << nickname << " not found." << std::endl;
	return NULL; // Exit if the client is not found
}

Channel	*Server::getChannelByName(const std::string &channelName)
{
	// Search for the channel by its name
	// Buscar el canal por su nombre
	if (getChannelList().find(channelName) != getChannelList().end())
		return getChannelList()[channelName];
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Channel not found: " << channelName << std::endl;
	return NULL; // Exit if the channel is not found
}
