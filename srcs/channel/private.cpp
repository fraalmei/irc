/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:55 by cagonzal          #+#    #+#             */
/*   Updated: 2025/05/23 13:10:56 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Private.hpp"

Private::Private(const std::string& name, const std::string& password)
    : AChannel(name), _password(password)
{
}

Private::~Private()
{
}

int Private::addMember(const Client& client, const std::string& password)
{
    if (password == this->_password)
    {
        _members.push_back(client);
        return 0; // Return 0 to indicate success
    }
    return 1; // Return 1 to indicate failure
}

void Private::removeMember(const std::string& nickname)
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

bool Private::isMember(const std::string& nickname) const
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

const std::string& Private::getName() const
{
    return _name;
}

const std::vector<Client>& Private::getMembers() const
{
    return _members;
}

const std::string& Private::getPassword() const
{
    return _password;
}

void Private::setPassword(const std::string& pass)
{
    _password = pass;
}