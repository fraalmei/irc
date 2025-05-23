/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelprivate.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:55 by cagonzal          #+#    #+#             */
/*   Updated: 2025/05/23 14:04:24 by cagonzal         ###   ########.fr       */
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

int ChannelPrivate::addMember(const Client& client, const std::string& password)
{
    if (password == this->_password)
    {
        _members.push_back(client);
        return 0; // Return 0 to indicate success
    }
    return 1; // Return 1 to indicate failure
}

void ChannelPrivate::removeMember(const std::string& nickname)
{
    for (std::vector<Client>::iterator it = _members.begin(); it != _members.end(); ++it)
    {
        if (it->getNickname() == nickname)
        {
            _members.erase(it);
            return;
        }
    }
}

bool ChannelPrivate::isMember(const std::string& nickname) const
{
    for (std::vector<Client>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    {
        if (it->getNickname() == nickname)
        {
            return true;
        }
    }
    return false;
}

const std::string& ChannelPrivate::getName() const
{
    return _name;
}

const std::vector<Client>& ChannelPrivate::getMembers() const
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