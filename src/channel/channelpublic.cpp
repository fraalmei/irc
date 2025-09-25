/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelpublic.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:27:10 by p                 #+#    #+#             */
/*   Updated: 2025/09/25 10:09:51 by cagonzal         ###   ########.fr       */
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

int ChannelPublic::addMember(const User* user)
{
	if (isMember(user->getNickname()))
		return -1; // Error: User is already a member
	_members.push_back(const_cast<User*>(user)); // Add the user to the members list
	for (std::vector<User*>::iterator it = _members.begin(); it != _members.end(); ++it)
		if ((*it)->getNickname() == user->getNickname())
			return -1; // Return -1 to indicate that the user is already a member
	_members.push_back(const_cast<User*>(user)); // Add the user to the members list
	return 0; // Success
}

int ChannelPublic::addMember(const User* user, std::string password)
{
	(void)password; // Ignore password for public channels
	(void)user; // Ignore user for public channels
	return 0; // Success
}

void ChannelPublic::removeMember(const std::string &nickname)
{
	for (std::vector<User*>::iterator it = _members.begin(); it != _members.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
		{
			_members.erase(it);

			for (std::vector<User*>::iterator it = _adminMembers.begin(); it != _adminMembers.end(); ++it)
			{
				if ((*it)->getNickname() == nickname)
					_adminMembers.erase(it);
			}
			return;
		}
	}
}

bool ChannelPublic::isMember(const std::string &nickname) const
{
	for (std::vector<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		if ((*it)->getNickname() == nickname)
			return true;
	return false;
}

bool ChannelPublic::isMember(const int &fd) const
{
	for (std::vector<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		if ((*it)->getFd() == fd)
			return true;
	return false;
}

const std::string &ChannelPublic::getName() const
{
	return _name;
}

const std::vector<User*> &ChannelPublic::getMembers() const
{
	return _members;
}

const std::vector<User*> &ChannelPublic::getAdminMembers() const
{
	return _adminMembers;
}
