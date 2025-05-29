/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelprivate.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:55 by cagonzal          #+#    #+#             */
/*   Updated: 2025/05/29 21:47:52 by p                ###   ########.fr       */
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

int ChannelPrivate::addMember(const Client* client, const std::string& password)
{
    if (password == this->_password)
    {
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
    return 1; // Return 1 to indicate failure
}

void ChannelPrivate::removeMember(const std::string& nickname)
{
    for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
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
    for (std::vector<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
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
    for (std::vector<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
        if ((*it)->getFd() == fd)
            return true;
    return false;
}

const std::string& ChannelPrivate::getName() const
{
    return _name;
}

const std::vector<Client*>& ChannelPrivate::getMembers() const
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