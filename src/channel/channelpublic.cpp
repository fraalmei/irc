// filepath: /home/arksh/Documentos/42 Madrid/irc/srcs/channel/public.cpp
#include "ChannelPublic.hpp"

ChannelPublic::ChannelPublic(const std::string &name) : AChannel(name)
{
}

ChannelPublic::~ChannelPublic()
{
}

int ChannelPublic::addMember(const Client &client, std::string password)
{
    (void)password; // Unused parameters
    _members.push_back(client);
    return 0; // Success
}

void ChannelPublic::removeMember(const std::string &nickname)
{
    for (std::vector<Client>::iterator it = _members.begin(); it != _members.end(); ++it) {
        if (it->getNickname() == nickname)
        {
            _members.erase(it);
            return;
        }
    }
}

bool ChannelPublic::isMember(const std::string &nickname) const
{
    for (std::vector<Client>::const_iterator it = _members.begin(); it != _members.end(); ++it)
        if (it->getNickname() == nickname)
            return true;
    return false;
}

const std::string &ChannelPublic::getName() const
{
    return _name;
}

const std::vector<Client> &ChannelPublic::getMembers() const
{
    return _members;
}