/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_executor.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 14:30:00 by cagonzal          #+#    #+#             */
/*   Updated: 2026/01/22 14:03:25 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include "../include/User.hpp"
#include "../include/Channel.hpp"
#include "../include/Commands.hpp"
#include "../include/IrcResponses.hpp"
#include "../include/ChannelUtils.hpp"
#include "../include/colors.hpp"

// ============================================================================
// Command Executors - Business Logic Layer
// Estas funciones contienen la lógica de negocio pura
// ============================================================================

int Commands::joinChannel(const std::string& channelName, User* new_client, Server& server)
{
	Channel* channel = server.getChannelByName(channelName);

	if (channel == NULL)
	{
		std::cout << CGRE << "[joinChannel]" << CRST << " " << channelName << " not found" << std::endl;
		// Crear nuevo canal público
		Channel* newChannel = new Channel(channelName);
		newChannel->addMember(new_client);
		newChannel->addOperator(new_client);
		server.getChannelList()[channelName] = newChannel;
		channel = newChannel;
		std::cout << CGRE << "[joinChannel]" << CRST << " Created channel: " << channelName << std::endl;
	}
	else
	{
		std::cout << CGRE << "[joinChannel]" << CRST << " Found channel: " << channel->getName() << std::endl;
		if (!channel->isMember(new_client->getFd()))
		{
			if (channel->addMember(new_client) != 0)
				std::cout << CGRE << "[joinChannel]" << CRST << " Error: User already in channel " << channelName << std::endl;
			std::cout << CGRE << "[joinChannel]" << CRST << " User " << new_client->getFd() << " joined " << channelName << std::endl;
		}
		else
		{
			std::cout << CGRE << "[joinChannel]" << CRST << " Error: User already in channel " << channelName << std::endl;
			std::stringstream ss;
			ss << ERR_NOTONCHANNEL;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + new_client->getNickname() + " " + channelName + " :You're not on that channel\r\n";
			send(new_client->getFd(), err.c_str(), err.length(), 0);
			return 1;
		}
	}

	// Send JOIN to all members
	std::string join_msg = ":" + new_client->getNickname() + "!" + new_client->getUsername() + "@localhost JOIN " + channel->getName() + "\r\n";
	for (std::vector<User*>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it)
	{
		send((*it)->getFd(), join_msg.c_str(), join_msg.length(), 0);
	}

	// Send RPL_NAMREPLY and RPL_ENDOFNAMES
	std::string names_list;
	for (std::vector<User*>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it)
	{
		if (!names_list.empty()) names_list += " ";
		names_list += (*it)->getNickname();
	}
	std::stringstream ss;
	ss << RPL_NAMREPLY;
	std::string namreply = ":" + std::string(ME) + " " + ss.str() + " " + new_client->getNickname() + " = " + channel->getName() + " :" + names_list + "\r\n";
	send(new_client->getFd(), namreply.c_str(), namreply.length(), 0);

	ss.str(""); ss << RPL_ENDOFNAMES;
	std::string endofnames = ":" + std::string(ME) + " " + ss.str() + " " + new_client->getNickname() + " " + channel->getName() + " :End of /NAMES list\r\n";
	send(new_client->getFd(), endofnames.c_str(), endofnames.length(), 0);

	return 0;
}

void Commands::sendMessage(const std::string& target, const std::string& message, User* user, Server& server)
{
	std::string actualTarget = target;

	// Check if target is a channel without #
	if (actualTarget[0] != '#')
	{
		std::string possibleChan = ChannelUtils::normalizeChannelName(actualTarget);
		Channel* chan = server.getChannelByName(possibleChan);
		if (chan && chan->isMember(user->getFd()))
			actualTarget = possibleChan;
	}

	if (actualTarget[0] == '#')
	{
		// Channel message
		Channel* chan = server.getChannelByName(actualTarget);
		if (!chan)
		{
			IrcResponses::sendErrorNoSuchChannel(user, actualTarget);
			return;
		}
		if (!chan->isMember(user->getFd()))
		{
			IrcResponses::sendErrorCannotSendToChan(user, actualTarget);
			return;
		}
		// Send to all members including sender
		std::string msg = IrcResponses::buildPrivmsgToChannel(user, actualTarget, message);
		ChannelUtils::broadcastToChannel(chan, msg);
	}
	else
	{
		// Private message
		User* target_user = server.getClientFdByNickname(actualTarget);
		if (!target_user)
		{
			IrcResponses::sendErrorNoSuchNick(user, actualTarget);
			return;
		}
		std::string msg = IrcResponses::buildPrivmsgToUser(user, actualTarget, message);
		send(target_user->getFd(), msg.c_str(), msg.length(), 0);
	}
}

void Commands::leaveChannel(const std::string& channelName, const std::string& partMsg, User* user, Server& server)
{
	Channel* chan = server.getChannelByName(channelName);
	
	if (!chan)
	{
		IrcResponses::sendErrorNoSuchChannel(user, channelName);
		return;
	}
	if (!chan->isMember(user->getFd()))
	{
		IrcResponses::sendErrorNotOnChannel(user, channelName);
		return;
	}
	
	// Send PART to all members
	std::string part_line = IrcResponses::buildPartMessage(user, channelName, partMsg);
	ChannelUtils::broadcastToChannel(chan, part_line);
	
	// Remove user from channel
	chan->removeMember(user);
}

void Commands::kickUser(const std::string& channelName, const std::string& targetNick, const std::string& reason, User* kicker, Server& server)
{
	Channel* chan = server.getChannelByName(channelName);
	
	if (!chan)
	{
		IrcResponses::sendErrorNoSuchChannel(kicker, channelName);
		return;
	}
	
	if (!chan->isOperator(kicker))
	{
		IrcResponses::sendErrorChanOpPrivsNeeded(kicker, channelName);
		return;
	}
	
	User* target = NULL;
	for (std::vector<User*>::const_iterator it = chan->getMembers().begin(); it != chan->getMembers().end(); ++it)
	{
		if ((*it)->getNickname() == targetNick)
		{
			target = *it;
			break;
		}
	}
	
	if (!target)
	{
		IrcResponses::sendErrorUserNotInChannel(kicker, targetNick, channelName);
		return;
	}
	
	// Send KICK to all members
	std::string kick_line = IrcResponses::buildKickMessage(kicker, channelName, targetNick, reason);
	ChannelUtils::broadcastToChannel(chan, kick_line);
	
	// Remove user
	chan->removeMember(target);
}

void Commands::setChannelMode(const std::string& channelName, const std::string& modeStr, const std::vector<std::string>& params, User* user, Server& server)
{
	Channel* chan = server.getChannelByName(channelName);
	
	if (!chan)
	{
		IrcResponses::sendErrorNoSuchChannel(user, channelName);
		return;
	}
	if (!chan->isMember(user->getFd()))
	{
		IrcResponses::sendErrorNotOnChannel(user, channelName);
		return;
	}
	
	if (modeStr.empty())
	{
		// Show modes
		std::string modes = "+";
		if (chan->isInviteOnly()) modes += "i";
		if (chan->isTopicProtected()) modes += "t";
		std::string mode_line = ":" + std::string(ME) + " 324 " + user->getNickname() + " " + channelName + " " + modes + "\r\n";
		send(user->getFd(), mode_line.c_str(), mode_line.length(), 0);
		return;
	}
	
	// Set modes - need operator
	if (!chan->isOperator(user))
	{
		IrcResponses::sendErrorChanOpPrivsNeeded(user, channelName);
		return;
	}
	
	char sign = modeStr[0];
	std::string mode = modeStr.substr(1);
	
	if (sign != '+' && sign != '-')
	{
		std::stringstream ss;
		ss << ERR_UNKNOWNMODE;
		std::string err = ":" + std::string(ME) + " " + ss.str() + " " + user->getNickname() + " " + mode + " :is unknown mode char to me\r\n";
		send(user->getFd(), err.c_str(), err.size(), 0);
		return;
	}
	
	bool set = (sign == '+');
	if (mode == "i")
	{
		chan->setInviteOnly(set);
	}
	else if (mode == "t")
	{
		chan->setTopicProtected(set);
	}
	else if (mode == "o")
	{
		if (params.size() < 3)
		{
			IrcResponses::sendErrorNeedMoreParams(user, "MODE");
			return;
		}
		std::string nick = params[2];
		User* target_user = NULL;
		for (std::vector<User*>::const_iterator it = chan->getMembers().begin(); it != chan->getMembers().end(); ++it)
		{
			if ((*it)->getNickname() == nick)
			{
				target_user = *it;
				break;
			}
		}
		if (!target_user)
		{
			IrcResponses::sendErrorUserNotInChannel(user, nick, channelName);
			return;
		}
		if (set)
			chan->addOperator(target_user);
		else
			chan->removeOperator(target_user);
	}
	else
	{
		std::stringstream ss;
		ss << ERR_UNKNOWNMODE;
		std::string err = ":" + std::string(ME) + " " + ss.str() + " " + user->getNickname() + " " + mode + " :is unknown mode char to me\r\n";
		send(user->getFd(), err.c_str(), err.size(), 0);
		return;
	}
	
	// Send MODE change to all members
	std::string mode_change = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost MODE " + channelName + " " + modeStr;
	if (mode == "o" && params.size() >= 3) 
		mode_change += " " + params[2];
	mode_change += "\r\n";
	ChannelUtils::broadcastToChannel(chan, mode_change);
}
