/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 00:00:00 by cagonzal          #+#    #+#             */
/*   Updated: 2026/01/22 13:23:35 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ChannelUtils.hpp"
#include "../include/Channel.hpp"
#include "../include/User.hpp"
#include <sys/socket.h>

std::string ChannelUtils::normalizeChannelName(const std::string& name)
{
	if (name.empty())
		return name;
	if (name[0] != '#')
		return "#" + name;
	return name;
}

void ChannelUtils::broadcastToChannel(Channel* channel, const std::string& message)
{
	const std::vector<User*>& members = channel->getMembers();
	for (std::vector<User*>::const_iterator it = members.begin(); it != members.end(); ++it)
	{
		send((*it)->getFd(), message.c_str(), message.length(), 0);
	}
}

void ChannelUtils::broadcastToChannelExcept(Channel* channel, const std::string& message, User* except)
{
	const std::vector<User*>& members = channel->getMembers();
	for (std::vector<User*>::const_iterator it = members.begin(); it != members.end(); ++it)
	{
		if (*it != except)
			send((*it)->getFd(), message.c_str(), message.length(), 0);
	}
}
