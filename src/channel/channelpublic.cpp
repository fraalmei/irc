/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelpublic.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:27:10 by p                 #+#    #+#             */
/*   Updated: 2025/06/12 17:27:11 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// filepath: /home/arksh/Documentos/42 Madrid/irc/srcs/channel/public.cpp
#include "ChannelPublic.hpp"

ChannelPublic::ChannelPublic(const std::string &name) : AChannel(name)
{
}

ChannelPublic::~ChannelPublic()
{
}

int ChannelPublic::addMember(const Client* client, std::string password)
{
	(void)password; // Unused parameters
	if (isMember(client->getNickname()))
		return -1; // Error: Client is already a member
	_members.push_back(const_cast<Client*>(client)); // Add the client to the members list
	for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
	{
		if ((*it)->getNickname() == client->getNickname())
		{
			return -1; // Return -1 to indicate that the client is already a member
		}
	}
	_members.push_back(const_cast<Client*>(client)); // Add the client to the members list
	return 0; // Success
}

void ChannelPublic::removeMember(const std::string &nickname)
{
	for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); ++it) {
		if ((*it)->getNickname() == nickname)
		{
			_members.erase(it);
			return;
		}
	}
}

bool ChannelPublic::isMember(const std::string &nickname) const
{
	for (std::vector<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		if ((*it)->getNickname() == nickname)
			return true;
	return false;
}

bool ChannelPublic::isMember(const int &fd) const
{
	for (std::vector<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		if ((*it)->getFd() == fd)
			return true;
	return false;
}

const std::string &ChannelPublic::getName() const
{
	return _name;
}

const std::vector<Client*> &ChannelPublic::getMembers() const
{
	return _members;
}
