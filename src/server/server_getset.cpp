/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_getset.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:31:13 by p                 #+#    #+#             */
/*   Updated: 2025/11/27 02:14:01 by p                ###   ########.fr       */
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

Channel	*Server::getChannelByName(const std::string &channelName)
{
	// Search for the channel by its name
	// Buscar el canal por su nombre
	if (getChannelList().find(channelName) != getChannelList().end())
		return getChannelList()[channelName];
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Channel not found: " << channelName << std::endl;
	return NULL; // Exit if the channel is not found
}
