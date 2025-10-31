/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_getset.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:31:13 by p                 #+#    #+#             */
/*   Updated: 2025/10/31 13:47:32 by cagonzal         ###   ########.fr       */
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
	std::cerr << "User with fd " << fd << " not found." << std::endl;
	return NULL; // Exit if the client is not found
}

OChannel	*Server::getChannelByName(const std::string &channelName)
{
	// Search for the channel by its name
	// Buscar el canal por su nombre
	if (getChannelList().find(channelName) != getChannelList().end())
		return getChannelList()[channelName];
	std::cerr << "Channel not found: " << channelName << std::endl;
	return NULL; // Exit if the channel is not found
}
