// filepath: /home/arksh/Documentos/42 Madrid/irc/srcs/channel/public.cpp
#include "Public.hpp"

Public::Public(const std::string &name) : AChannel(name)
{
}

Public::~Public()
{
}

int Public::addMember(const Client &client, std::string password)
{
    (void)password; // Unused parameters
    _members.push_back(client);
    return 0; // Success
}

void Public::removeMember(const std::string &nickname)
{
    for (std::vector<Client>::iterator it = _members.begin(); it != _members.end(); ++it) {
        if (it->getNickname() == nickname)
        {
            _members.erase(it);
            return;
        }
    }
}

bool Public::isMember(const std::string &nickname) const
{
    for (std::vector<Client>::const_iterator it = _members.begin(); it != _members.end(); ++it)
        if (it->getNickname() == nickname)
            return true;
    return false;
}

const std::string &Public::getName() const
{
    return _name;
}

const std::vector<Client> &Public::getMembers() const
{
    return _members;
}