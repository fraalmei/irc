/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 00:00:00 by cagonzal          #+#    #+#             */
/*   Updated: 2026/01/22 14:56:46 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "msg_handler.hpp"

class Server;

/// @brief Declaraciones de todas las funciones de comandos IRC
namespace Commands
{
	// Funciones de validación (entry points)
	void commandJoin(msg_handler::t_command& command, Server& server);
	void commandPrivmsg(msg_handler::t_command& command, Server& server);
	void commandPart(msg_handler::t_command& command, Server& server);
	void commandWho(msg_handler::t_command& command, Server& server);
	void commandKick(msg_handler::t_command& command, Server& server);
	void commandInvite(msg_handler::t_command& command, Server& server);
	void commandTopic(msg_handler::t_command& command, Server& server);
	void commandMode(msg_handler::t_command& command, Server& server);

	// Funciones de ejecución de lógica
	int joinChannel(const std::string& channelName, User* user, Server& server);
	void sendMessage(const std::string& target, const std::string& message, User* user, Server& server);
	void leaveChannel(const std::string& channelName, const std::string& partMsg, User* user, Server& server);
	void kickUser(const std::string& channelName, const std::string& targetNick, const std::string& reason, User* kicker, Server& server);
	void setChannelMode(const std::string& channelName, const std::string& modeStr, const std::vector<std::string>& params, User* user, Server& server);
}

#endif
