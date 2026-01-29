/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_handler.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 14:30:00 by cagonzal          #+#    #+#             */
/*   Updated: 2026/01/29 10:08:06 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include "../include/User.hpp"
#include "../include/Channel.hpp"
#include "../include/Commands.hpp"
#include "../include/IrcResponses.hpp"
#include "../include/ChannelUtils.hpp"

// ============================================================================
// Command Handlers - Validation Layer
// Estas funciones validan parámetros y llaman a la lógica de negocio
// ============================================================================

void Commands::commandJoin(msg_handler::t_command& command, Server& server)
{
	if (command.params.empty())
	{
		IrcResponses::sendErrorNeedMoreParams(command.user, "JOIN");
		return;
	}
	std::string channelName = ChannelUtils::normalizeChannelName(command.params[0]);
	Commands::joinChannel(channelName, command.user, server);
}

void Commands::commandPrivmsg(msg_handler::t_command& command, Server& server)
{
	if (command.params.size() < 2)
	{
		IrcResponses::sendErrorNoRecipient(command.user, "PRIVMSG");
		return;
	}
	
	std::string target = command.params[0];
	std::string message = command.params[1];
	Commands::sendMessage(target, message, command.user, server);
}

void Commands::commandPart(msg_handler::t_command& command, Server& server)
{
	if (command.params.empty())
	{
		IrcResponses::sendErrorNeedMoreParams(command.user, "PART");
		return;
	}
	
	std::string channelName = ChannelUtils::normalizeChannelName(command.params[0]);
	std::string part_msg = "Left the channel";
	if (command.params.size() > 1)
	{
		part_msg = command.params[1];
		if (!part_msg.empty() && part_msg[0] == ':') 
			part_msg = part_msg.substr(1);
	}
	
	Commands::leaveChannel(channelName, part_msg, command.user, server);
}

void Commands::commandWho(msg_handler::t_command& command, Server& server)
{
	if (command.params.empty())
	{
		IrcResponses::sendErrorNeedMoreParams(command.user, "WHO");
		return;
	}
	
	std::string channelName = ChannelUtils::normalizeChannelName(command.params[0]);
	Channel* chan = server.getChannelByName(channelName);
	
	if (!chan)
	{
		IrcResponses::sendErrorNoSuchChannel(command.user, channelName);
		return;
	}
	
	// Send RPL_WHOREPLY for each member
	for (std::vector<User*>::const_iterator it = chan->getMembers().begin(); it != chan->getMembers().end(); ++it)
	{
		std::string flags = "H"; // Here
		if (chan->isOperator(*it)) flags += "@"; // Operator
		std::stringstream ss;
		ss << RPL_WHOREPLY;
		std::string who_line = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " " + (*it)->getUsername() + " localhost " + std::string(ME) + " " + (*it)->getNickname() + " " + flags + " :0 " + (*it)->getRealname() + "\r\n";
		send(command.user->getFd(), who_line.c_str(), who_line.length(), 0);
	}
	
	// Send RPL_ENDOFWHO
	std::stringstream ss;
	ss << RPL_ENDOFWHO;
	std::string end_line = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :End of /WHO list\r\n";
	send(command.user->getFd(), end_line.c_str(), end_line.length(), 0);
}

void Commands::commandKick(msg_handler::t_command& command, Server& server)
{
	if (command.params.size() < 2)
	{
		IrcResponses::sendErrorNeedMoreParams(command.user, "KICK");
		return;
	}
	
	std::string channelName = ChannelUtils::normalizeChannelName(command.params[0]);
	std::string targetNick = command.params[1];
	std::string reason = "Kicked";
	if (command.params.size() > 2)
	{
		reason = command.params[2];
		if (!reason.empty() && reason[0] == ':') 
			reason = reason.substr(1);
	}
	
	Commands::kickUser(channelName, targetNick, reason, command.user, server);
}

void Commands::commandInvite(msg_handler::t_command& command, Server& server)
{
	if (command.params.size() < 2)
	{
		IrcResponses::sendErrorNeedMoreParams(command.user, "INVITE");
		return;
	}
	
	std::string targetNick = command.params[0];
	std::string channelName = ChannelUtils::normalizeChannelName(command.params[1]);
	
	User* target = server.getClientFdByNickname(targetNick);
	if (!target)
	{
		IrcResponses::sendErrorNoSuchNick(command.user, targetNick);
		return;
	}
	
	Channel* chan = server.getChannelByName(channelName);
	if (!chan)
	{
		IrcResponses::sendErrorNoSuchChannel(command.user, channelName);
		return;
	}
	
	if (!chan->isMember(command.user->getFd()))
	{
		IrcResponses::sendErrorNotOnChannel(command.user, channelName);
		return;
	}
	
	if (chan->isMember(target->getFd()))
	{
		std::string err = ":" + std::string(ME) + " 443 " + command.user->getNickname() + " " + targetNick + " " + channelName + " :is already on channel\r\n";
		send(command.user->getFd(), err.c_str(), err.size(), 0);
		return;
	}
	
	// Send RPL_INVITING
	std::stringstream ss;
	ss << RPL_INVITING;
	std::string invite_line = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + targetNick + " " + channelName + "\r\n";
	send(command.user->getFd(), invite_line.c_str(), invite_line.length(), 0);
	
	// Send invite message to target
	std::string invite_msg = ":" + command.user->getNickname() + "!" + command.user->getUsername() + "@localhost INVITE " + targetNick + " :" + channelName + "\r\n";
	send(target->getFd(), invite_msg.c_str(), invite_msg.length(), 0);

	// Add to invite list in the channel
	chan->addInvitation(target);
}

void Commands::commandTopic(msg_handler::t_command& command, Server& server)
{
	if (command.params.empty())
	{
		IrcResponses::sendErrorNeedMoreParams(command.user, "TOPIC");
		return;
	}
	
	std::string channelName = ChannelUtils::normalizeChannelName(command.params[0]);
	Channel* chan = server.getChannelByName(channelName);
	
	if (!chan)
	{
		IrcResponses::sendErrorNoSuchChannel(command.user, channelName);
		return;
	}
	if (!chan->isMember(command.user->getFd()))
	{
		IrcResponses::sendErrorNotOnChannel(command.user, channelName);
		return;
	}
	
	if (command.params.size() == 1)
	{
		// Show topic
		if (chan->getTopic().empty())
		{
			std::string no_topic = ":" + std::string(ME) + " 331 " + command.user->getNickname() + " " + channelName + " :No topic is set\r\n";
			send(command.user->getFd(), no_topic.c_str(), no_topic.length(), 0);
		}
		else
		{
			std::stringstream ss;
			ss << RPL_TOPIC;
			std::string topic_line = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :" + chan->getTopic() + "\r\n";
			send(command.user->getFd(), topic_line.c_str(), topic_line.length(), 0);
		}
	}
	else
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Trying to set topic. isTopicProtected flag is set to: " << chan->isTopicProtected() << " and chan->isOperator(command.user) is set to " << chan->isOperator(command.user) << std::endl;

		// Set topic
		if (chan->isTopicProtected() && !chan->isOperator(command.user))
		{
			IrcResponses::sendErrorChanOpPrivsNeeded(command.user, channelName);
			return;
		}
		std::string new_topic = command.params[1];
		if (!new_topic.empty() && new_topic[0] == ':') 
			new_topic = new_topic.substr(1);
		chan->setTopic(new_topic);
		// Send TOPIC change to all members
		std::string topic_change = ":" + command.user->getNickname() + "!" + command.user->getUsername() + "@localhost TOPIC " + channelName + " :" + new_topic + "\r\n";
		ChannelUtils::broadcastToChannel(chan, topic_change);
	}
}

void Commands::commandMode(msg_handler::t_command& command, Server& server)
{
	if (command.params.empty())
	{
		IrcResponses::sendErrorNeedMoreParams(command.user, "MODE");
		return;
	}
	
	std::string target = command.params[0];
	if (target[0] == '#')
	{
		// Channel mode
		std::string channelName = ChannelUtils::normalizeChannelName(target);
		std::string modeStr = command.params.size() > 1 ? command.params[1] : "";
		Commands::setChannelMode(channelName, modeStr, command.params, command.user, server);
	}
	else
	{
		// User mode - not implemented
		std::string err = ":" + std::string(ME) + " 501 " + command.user->getNickname() + " :Unknown MODE flag\r\n";
		send(command.user->getFd(), err.c_str(), err.size(), 0);
	}
}
