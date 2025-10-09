/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OChannel.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:27:10 by p                 #+#    #+#             */
/*   Updated: 2025/10/09 13:50:08 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// filepath: /home/arksh/Documentos/42 Madrid/irc/srcs/channel/public.cpp
#include "OChannel.hpp"

int OChannel::addMember(const User* user)
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

int OChannel::addMember(const User* user, const std::string& password)
{
	if (password == this->_password)
	{
		_members.push_back(const_cast<User*>(user)); // Add the user to the members list
		for (std::vector<User*>::iterator it = _members.begin(); it != _members.end(); ++it)
		{
			if ((*it)->getNickname() == user->getNickname())
				return -1; // Return -1 to indicate that the user is already a member
		}
		_members.push_back(const_cast<User*>(user)); // Add the user to the members list
		return 0; // Success
	}
	return 1; // Return 1 to indicate failure
}

void OChannel::removeMember(const std::string &nickname)
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

bool OChannel::isMember(const std::string &nickname) const
{
	for (std::vector<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		if ((*it)->getNickname() == nickname)
			return true;
	return false;
}

bool OChannel::isMember(const int &fd) const
{
	for (std::vector<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		if ((*it)->getFd() == fd)
			return true;
	return false;
}

const std::string &OChannel::getName() const
{
	return _name;
}

const std::vector<User*> &OChannel::getMembers() const
{
	return _members;
}

const std::vector<User*> &OChannel::getAdminMembers() const
{
	return _adminMembers;
}
