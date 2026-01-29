/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:27:10 by p                 #+#    #+#             */
/*   Updated: 2025/10/31 14:29:50 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// filepath: /home/arksh/Documentos/42 Madrid/irc/srcs/channel/public.cpp
#include "Channel.hpp"
#include <algorithm>

int Channel::addMember(const User* user)
{
	if (isMember(user->getNickname()))
		return -1; // Error: User is already a member
	_members.push_back(const_cast<User*>(user)); // Add the user to the members list
	return 0; // Success
}

int Channel::addMember(const User* user, const std::string& password)
{
	if (password == this->_password)
	{
		if (isMember(user->getNickname()))
			return -1; // Error: User is already a member
		_members.push_back(const_cast<User*>(user)); // Add the user to the members list
		return 0; // Success
	}
	return 1; // Return 1 to indicate failure
}

int addInvitation(const User* user)
{
	if (isInvited(user->getNickname()))
		return -1; // Error: User is already in invite list
	_invitedUsers.push_back(const_cast<User*>(user));
	return 0;
}

void Channel::removeMember(const std::string &nickname)
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

void removeInvitation(const User* user)
{
	for (std::vector<User*>::iterator it = _invitedUsers.begin(); it != _invitedUsers.end(); ++it)
	{
		if (*it == user)
			_invitedUsers.erase(it);
	}
}

bool Channel::isMember(const std::string &nickname) const
{
	for (std::vector<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		if ((*it)->getNickname() == nickname)
			return true;
	return false;
}

bool Channel::isMember(const int &fd) const
{
	for (std::vector<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		if ((*it)->getFd() == fd)
			return true;
	return false;
}

bool Channel::isAdminMember(const std::string &nickname) const
{
	for (std::vector<User*>::const_iterator it = _adminMembers.begin(); it != _adminMembers.end(); ++it)
		if ((*it)->getNickname() == nickname)
			return true;
	return false;
}
bool Channel::isInvited(const std::string &nickname) const
{
	for (std::vector<User*>::const_iterator it = _invitedUsers.begin(); it != _invitedUsers.end(); ++it)
		if ((*it)->getNickname() == nickname)
			return true;
	return false;
}

void Channel::removeMember(User* user)
{
	_members.erase(std::remove(_members.begin(), _members.end(), user), _members.end());
	_adminMembers.erase(std::remove(_adminMembers.begin(), _adminMembers.end(), user), _adminMembers.end());
}

bool Channel::isOperator(User* user) const
{
	return std::find(_adminMembers.begin(), _adminMembers.end(), user) != _adminMembers.end();
}

void Channel::addOperator(User* user)
{
	if (!isOperator(user))
		_adminMembers.push_back(user);
}

void Channel::removeOperator(User* user)
{
	_adminMembers.erase(std::remove(_adminMembers.begin(), _adminMembers.end(), user), _adminMembers.end());
}
