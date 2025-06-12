/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_getters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:31:13 by p                 #+#    #+#             */
/*   Updated: 2025/06/12 16:54:38 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int		Server::get_server_fd()
{
	return this->_server_fd;
}

int		Server::get_fd_max()
{
	return this->_fd_max;
}

fd_set	Server::get_master_set()
{
	return this->_master_set;
}

fd_set	Server::get_read_fds()
{
	return this->_read_fds;
}

const std::map<std::string, AChannel*>&		Server::getConstChannelList() const
{
	return this->_channel_list;
}

std::map<std::string, AChannel*>&			Server::getChannelList()
{
	return this->_channel_list;
}

const std::map<int, Client*>&				Server::getConstClientList() const
{
	return this->_client_list;
}

std::map<int, Client*>&						Server::getClientList()
{
	return this->_client_list;
}

Client	*Server::getClientByFd(int fd)
{
	// Search for the client by its file descriptor
	// Buscar el cliente por su descriptor de archivo
	for(std::map<int, Client*>::const_iterator it = getConstClientList().begin(); it != getConstClientList().end(); ++it)
	{
		if (it->first == fd)
		{
			return it->second; // Return the found client
		}
	}
	std::cerr << "Client with fd " << fd << " not found." << std::endl;
	return NULL; // Exit if the client is not found
}

AChannel	*Server::getChannelByName(const std::string &channelName)
{
	// Search for the channel by its name
	// Buscar el canal por su nombre
	if (getChannelList().find(channelName) != getChannelList().end())
		return getChannelList()[channelName];
	std::cerr << "Channel not found: " << channelName << std::endl;
	return NULL; // Exit if the channel is not found
}
