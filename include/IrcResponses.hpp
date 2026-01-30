/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcResponses.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 00:00:00 by cagonzal          #+#    #+#             */
/*   Updated: 2026/01/30 18:34:12 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCRESPONSES_HPP
# define IRCRESPONSES_HPP

# include <string>
# include <sstream>
# include <sys/socket.h>

class User;

/// @brief Build and send replies to clients
class IrcResponses
{
	public:
		// Send error responses
		static void sendErrorNeedMoreParams(User* user, const std::string& command);
		static void sendErrorNoSuchChannel(User* user, const std::string& channel);
		static void sendErrorNotOnChannel(User* user, const std::string& channel);
		static void sendErrorCannotSendToChan(User* user, const std::string& channel);
		static void sendErrorNoSuchNick(User* user, const std::string& nick);
		static void sendErrorUserNotInChannel(User* user, const std::string& nick, const std::string& channel);
		static void sendErrorChanOpPrivsNeeded(User* user, const std::string& channel);
		static void sendErrorNoRecipient(User* user, const std::string& command);
		static void	sendErrorCannotJoinInvite(User* user,  const std::string& channel);
		static void	sendErrorCannotJoinSize(User* user,  const std::string& channel);
		static void sendErrorNeedPassword(User* user, const std::string& command);
		// Send welcome messages
		static void sendWelcomeMessages(User* user);
		
		// Build IRC messages
		static std::string buildPrivmsgToChannel(User* sender, const std::string& channel, const std::string& message);
		static std::string buildPrivmsgToUser(User* sender, const std::string& target, const std::string& message);
		static std::string buildJoinMessage(User* user, const std::string& channel);
		static std::string buildPartMessage(User* user, const std::string& channel, const std::string& reason);
		static std::string buildKickMessage(User* kicker, const std::string& channel, const std::string& target, const std::string& reason);
		
	private:
		IrcResponses();
		~IrcResponses();
		
		static std::string getServerName();
		static void sendMessage(int fd, const std::string& message);
};

#endif
