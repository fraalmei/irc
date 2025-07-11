/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelprivate.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:26:42 by p                 #+#    #+#             */
/*   Updated: 2025/07/11 12:36:59 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ChannelPrivate.hpp"

ChannelPrivate::ChannelPrivate(const std::string& name, const std::string& password)
	: AChannel(name), _password(password)
{
}

ChannelPrivate::~ChannelPrivate()
{
}

int ChannelPrivate::addMember(const User* user)
{
	(void)user; // Ignore user for private channels
	return -1;
}

int ChannelPrivate::addMember(const User* user, const std::string& password)
{
	if (password == this->_password)
	{
		_members.push_back(const_cast<User*>(user)); // Add the user to the members list
		for (std::vector<User*>::iterator it = _members.begin(); it != _members.end(); ++it)
		{
			if ((*it)->getNickname() == user->getNickname())
			{
				return -1; // Return -1 to indicate that the user is already a member
			}
		}
		_members.push_back(const_cast<User*>(user)); // Add the user to the members list
		return 0; // Success
	}
	return 1; // Return 1 to indicate failure
}

void ChannelPrivate::removeMember(const std::string& nickname)
{
	for (std::vector<User*>::iterator it = _members.begin(); it != _members.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
		{
			_members.erase(it);
			return;
		}
	}
}

bool ChannelPrivate::isMember(const std::string& nickname) const
{
	for (std::vector<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
		{
			return true;
		}
	}
	return false;
}

bool ChannelPrivate::isMember(const int &fd) const
{
	for (std::vector<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		if ((*it)->getFd() == fd)
			return true;
	return false;
}

const std::string& ChannelPrivate::getName() const
{
	return _name;
}

const std::vector<User*>& ChannelPrivate::getMembers() const
{
	return _members;
}

const std::string& ChannelPrivate::getPassword() const
{
	return _password;
}

void ChannelPrivate::setPassword(const std::string& pass)
{
	_password = pass;
}
