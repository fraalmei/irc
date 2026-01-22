/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcResponses.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 00:00:00 by cagonzal          #+#    #+#             */
/*   Updated: 2026/01/22 13:49:46 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/IrcResponses.hpp"
#include "../include/User.hpp"
#include "../include/msg_handler.hpp"

std::string IrcResponses::getServerName()
{
	return ME;
}

void IrcResponses::sendMessage(int fd, const std::string& message)
{
	send(fd, message.c_str(), message.size(), 0);
}

void IrcResponses::sendErrorNeedMoreParams(User* user, const std::string& command)
{
	std::stringstream ss;
	ss << ERR_NEEDMOREPARAMS;
	std::string err = ":" + getServerName() + " " + ss.str() + " " + user->getNickname() + " " + command + " :Not enough parameters\r\n";
	sendMessage(user->getFd(), err);
}

void IrcResponses::sendErrorNoSuchChannel(User* user, const std::string& channel)
{
	std::stringstream ss;
	ss << ERR_NOSUCHCHANNEL;
	std::string err = ":" + getServerName() + " " + ss.str() + " " + user->getNickname() + " " + channel + " :No such channel\r\n";
	sendMessage(user->getFd(), err);
}

void IrcResponses::sendErrorNotOnChannel(User* user, const std::string& channel)
{
	std::stringstream ss;
	ss << ERR_NOTONCHANNEL;
	std::string err = ":" + getServerName() + " " + ss.str() + " " + user->getNickname() + " " + channel + " :You're not on that channel\r\n";
	sendMessage(user->getFd(), err);
}

void IrcResponses::sendErrorCannotSendToChan(User* user, const std::string& channel)
{
	std::stringstream ss;
	ss << ERR_CANNOTSENDTOCHAN;
	std::string err = ":" + getServerName() + " " + ss.str() + " " + user->getNickname() + " " + channel + " :Cannot send to channel\r\n";
	sendMessage(user->getFd(), err);
}

void IrcResponses::sendErrorNoSuchNick(User* user, const std::string& nick)
{
	std::stringstream ss;
	ss << ERR_NOSUCHNICK;
	std::string err = ":" + getServerName() + " " + ss.str() + " " + user->getNickname() + " " + nick + " :No such nick/channel\r\n";
	sendMessage(user->getFd(), err);
}

void IrcResponses::sendErrorUserNotInChannel(User* user, const std::string& nick, const std::string& channel)
{
	std::stringstream ss;
	ss << ERR_USERNOTINCHANNEL;
	std::string err = ":" + getServerName() + " " + ss.str() + " " + user->getNickname() + " " + nick + " " + channel + " :They aren't on that channel\r\n";
	sendMessage(user->getFd(), err);
}

void IrcResponses::sendErrorChanOpPrivsNeeded(User* user, const std::string& channel)
{
	std::stringstream ss;
	ss << ERR_CHANOPRIVSNEEDED;
	std::string err = ":" + getServerName() + " " + ss.str() + " " + user->getNickname() + " " + channel + " :You're not channel operator\r\n";
	sendMessage(user->getFd(), err);
}

void IrcResponses::sendErrorNoRecipient(User* user, const std::string& command)
{
	std::stringstream ss;
	ss << ERR_NORECIPIENT;
	std::string err = ":" + getServerName() + " " + ss.str() + " " + user->getNickname() + " :No recipient given (" + command + ")\r\n";
	sendMessage(user->getFd(), err);
}

std::string IrcResponses::buildPrivmsgToChannel(User* sender, const std::string& channel, const std::string& message)
{
	return ":" + sender->getNickname() + "!" + sender->getUsername() + "@localhost PRIVMSG " + channel + " :" + message + "\r\n";
}

std::string IrcResponses::buildPrivmsgToUser(User* sender, const std::string& target, const std::string& message)
{
	return ":" + sender->getNickname() + "!" + sender->getUsername() + "@localhost PRIVMSG " + target + " :" + message + "\r\n";
}

std::string IrcResponses::buildJoinMessage(User* user, const std::string& channel)
{
	return ":" + user->getNickname() + "!" + user->getUsername() + "@localhost JOIN :" + channel + "\r\n";
}

std::string IrcResponses::buildPartMessage(User* user, const std::string& channel, const std::string& reason)
{
	return ":" + user->getNickname() + "!" + user->getUsername() + "@localhost PART " + channel + " :" + reason + "\r\n";
}

std::string IrcResponses::buildKickMessage(User* kicker, const std::string& channel, const std::string& target, const std::string& reason)
{
	return ":" + kicker->getNickname() + "!" + kicker->getUsername() + "@localhost KICK " + channel + " " + target + " :" + reason + "\r\n";
}
