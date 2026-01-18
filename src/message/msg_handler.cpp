/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 11:38:21 by p                 #+#    #+#             */
/*   Updated: 2026/01/18 15:00:51 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"
#include "../include/User.hpp"
#include "../include/validNames.hpp"
#include "../include/colors.hpp"

int msg_handler::handle_buffer(std::string buffer, User *user)
{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " buffer -> " << buffer << std::endl;

	int msg_result = user->addToBuffer(buffer);
	if (msg_result == -1)
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Se ha recibido un mensaje cortado sin fin de línea." << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario: " << user->getFd() << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer: " << user->getBuffer() << "." << std::endl;
		return 1;
	}
	else if (msg_result == 0)
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Se ha recibido un mensaje vacio." << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario: " << user->getFd() << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer: " << user->getBuffer() << "." << std::endl;
		return 1;
	}
	else
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Se ha recibido un mensaje y está completo para parsear." << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario: " << user->getFd() << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer: " << user->getBuffer() << "." << std::endl;
	}
	return 0;
}

int	msg_handler::print_command(msg_handler::t_command command)
{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Command: '" << command.command << "'" << std::endl;
	for (std::vector<std::string>::const_iterator it = command.params.begin(); it != command.params.end(); ++it)
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Param: '" << *it << "'" << std::endl;
	}
	return 0;
}

msg_handler::t_command msg_handler::parse_msg(User* user)
{
	msg_handler::t_command	command;

	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CYEL << "Buffer: " << CRST << "\"" << user->getBuffer() << "\"" << std::endl;
	command.actual_line = user->getBuffer().substr(0, user->getBuffer().find("\n"));
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CYEL << "Extracted line: " << CRST << "\"" << command.actual_line << "\"" << std::endl;
	user->removeFromBuffer(command.actual_line.length() + 1); // +1 to remove \n
	if (command.actual_line.find('\r') != std::string::npos)
		command.actual_line.erase(command.actual_line.length() - 1);
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CYEL << "Resting buffer: " << CRST << "\"" << user->getBuffer() << "\"" << std::endl;
		/* if (line.empty())
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje vacío." << std::endl;
		command.user = NULL;
	}
	else
	{
		if (user->getBuffer().find("\r\n") != std::string::npos)
			user->setBuffer(user->getBuffer().substr(user->getBuffer().find("\r\n") + 2));
		command.user = user;
		ss.str(line);
		ss.clear();
	// --- Estados ---
	// 0: Start/Waiting for Prefix or Command
	// 1: Reading Command
	// 2: Reading Middle Parameters
	// 3: Reading Trailing Parameter
	// La lógica de parsing de IRC está basada en espacios y el carácter ':'
		while (ss >> token)
		{
			if (state == 0)
			{
				// Estado 0: Inicio
				if (token[0] == ':')
				{
					// Transición: ':' -> Estado 1 (Command)
					// Consume el prefijo y el token de prefijo completo
					command.prefix = token.substr(1); // Guarda el prefijo (sin los ':')
					state = 1; // El siguiente token DEBE ser el comando
				}
				else
				{
					// Transición: Carácter -> Estado 1 (Command)
					command.command = token;
					state = 2; // El siguiente token será un parámetro
				}
			}
			else if (state == 1)
			{
				// Estado 1: Lectura del Comando (después de un prefijo)
				// Transición: Carácter -> Estado 2 (Middle Params)
				command.command = token;
				state = 2;
			}
			else if (state == 2)
			{
				// Estado 2: Lectura de Parámetros Medios
				if (token[0] == ':')
				{
					// Transición: ':' -> Estado 3 (Trailing Param)
					
					// Lee el resto de la línea (incluyendo espacios) para el parámetro final
					std::string trailing_part = token.substr(1); // Primer parte del trailing
					std::string rest_of_line;
					
					// Necesitamos la posición actual en el stringstream para leer el resto de la línea
					// Esta es la parte "no-canónica" del AFD, donde el parser salta la lectura basada en '>>'
					// y lee el resto de la línea.
					std::getline(ss, rest_of_line); 
					
					// El parámetro final incluye el espacio antes y después (si hay)
					// y el resto del contenido de la línea.
					if (!rest_of_line.empty() && rest_of_line[0] == ' ')
						trailing_part += rest_of_line.substr(1); // Quita el espacio extra de getline
					else
						trailing_part += rest_of_line;
					command.params.push_back(trailing_part);
					state = 3; // Final de la lógica, cualquier otra cosa es basura
					break; // Salir del bucle, el resto de la línea ya fue consumida
				}
				else
				{
					// Transición: Carácter -> Estado 2 (Middle Params) (sigue leyendo parámetros medios)
					command.params.push_back(token);
				}
			}
			// El Estado 3 (Trailing Param) es el estado de ACEPTACIÓN y no procesa más tokens
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Comando parseado: '" << command.command << "' con " << command.params.front() << " parámetros." << std::endl;
		}
	}
	msg_handler::print_command(command);
	return command; */
	if (command.actual_line.empty())
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje vacío." << std::endl;
		command.user = NULL;
	}
	else
	{
		command.user = user;
		command.command = command.actual_line.substr(0, command.actual_line.find(' '));
		command.actual_line = command.actual_line.substr(command.actual_line.find(' ') + 1);
		command.params.push_back(command.actual_line.substr(0, command.actual_line.find(' ')));
		msg_handler::print_command(command);
	}
	return command;
}

void msg_handler::execute_command(msg_handler::t_command command, Server &server)
{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Ejecutando comando: " << command.command << std::endl;
	if (!command.user->isAuthenticated() && command.command == "USER")
	{
		if (handle_username(command))
			return;
		server.set_Authentication(command.user);
	}
	else if (!command.user->isAuthenticated() && command.command == "NICK")
	{
		if (handle_nickname(command))
			return;
		// server.set_Authentication(command.user);
	}
	else if (!command.user->isAuthenticated() && command.command == "PASS")
	{
		if (handle_password(command, &server))
			return;
		// server.set_Authentication(command.user);
	}
	else if (command.command == "CAP")
	{
		command.user->setCapability(command.params.front().substr(0, command.params.front().find(' ')));
		command.user->setVersion(std::atoi(command.params.front().substr(command.params.front().find(' ') + 1).c_str()));
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Capability set to: " << command.user->getCapability() << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Version set to: " << command.user->getVersion() << std::endl;
		// Respond to CAP negotiation
		std::string cap_response = "CAP * NAK\r\n";
		send(command.user->getFd(), cap_response.c_str(), cap_response.size(), 0);
	}
	else if (command.command == "JOIN")
	{
		if (command.params.empty())
		{
			std::string err = std::string(":") + ME + " 461 JOIN :Not enough parameters\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string channelName = command.params.front();
		if (channelName[0] != '#')
			channelName = "#" + channelName;
		
		joinChannel(channelName, command.user, server);
	}
	else if (command.command == "QUIT")
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Client with fd " << command.user->getFd() << " sent QUIT command." << std::endl;
		server.ClearClients(command.user->getFd());
	}
	else if (command.command == "PRIVMSG")
	{
		std::stringstream ss;
		if (command.params.empty())
		{
			ss << ERR_NORECIPIENT;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " :No recipient given (PRIVMSG)\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string target = command.params.front();
		if (!command.params.empty()) command.params.erase(command.params.begin());
		// Extract message from the remaining line
		size_t space_pos = command.actual_line.find(' ');
		if (space_pos == std::string::npos)
		{
			ss.str(""); ss << ERR_NOTEXTTOSEND;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " :No text to send\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string message_part = command.actual_line.substr(space_pos + 1);
		if (message_part.empty() || message_part[0] != ':')
		{
			ss.str(""); ss << ERR_NOTEXTTOSEND;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " :No text to send\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string message = message_part.substr(1); // Remove leading ':'

		// Check if target is a channel without #
		if (target[0] != '#')
		{
			std::string possibleChan = "#" + target;
			Channel* chan = server.getChannelByName(possibleChan);
			if (chan && chan->isMember(command.user->getFd()))
			{
				target = possibleChan;
			}
		}

		if (target[0] == '#')
		{
			// Channel message
			Channel* chan = server.getChannelByName(target);
			if (!chan)
			{
				ss.str(""); ss << ERR_NOSUCHCHANNEL;
				std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + target + " :No such channel\r\n";
				send(command.user->getFd(), err.c_str(), err.size(), 0);
				return;
			}
			if (!chan->isMember(command.user->getFd()))
			{
				ss.str(""); ss << ERR_CANNOTSENDTOCHAN;
				std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + target + " :Cannot send to channel\r\n";
				send(command.user->getFd(), err.c_str(), err.size(), 0);
				return;
			}
			// Send to all members including sender
			std::string msg = ":" + command.user->getNickname() + "!" + command.user->getUsername() + "@localhost PRIVMSG " + target + " :" + message + "\r\n";
			for (std::vector<User*>::const_iterator it = chan->getMembers().begin(); it != chan->getMembers().end(); ++it)
			{
				send((*it)->getFd(), msg.c_str(), msg.length(), 0);
			}
		}
		else
		{
			// Private message
			User* target_user = server.getClientFdByNickname(target);
			if (!target_user)
			{
				ss.str(""); ss << ERR_NOSUCHNICK;
				std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + target + " :No such nick/channel\r\n";
				send(command.user->getFd(), err.c_str(), err.size(), 0);
				return;
			}
			std::string msg = ":" + command.user->getNickname() + "!" + command.user->getUsername() + "@localhost PRIVMSG " + target + " :" + message + "\r\n";
			send(target_user->getFd(), msg.c_str(), msg.length(), 0);
		}
	}
	else if (command.command == "PART")
	{
		if (command.params.empty())
		{
			std::stringstream ss;
			ss << ERR_NEEDMOREPARAMS;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " PART :Not enough parameters\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string channelName = command.params.front();
		if (channelName[0] != '#')
			channelName = "#" + channelName;
		Channel* chan = server.getChannelByName(channelName);
		if (!chan)
		{
			std::stringstream ss;
			ss << ERR_NOSUCHCHANNEL;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :No such channel\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		if (!chan->isMember(command.user->getFd()))
		{
			std::stringstream ss;
			ss << ERR_NOTONCHANNEL;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :You're not on that channel\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		// Extract message if any
		std::string part_msg = "Left the channel";
		if (command.params.size() > 1)
		{
			part_msg = command.params[1];
			if (!part_msg.empty() && part_msg[0] == ':') part_msg = part_msg.substr(1);
		}
		// Send PART to all members
		std::string part_line = ":" + command.user->getNickname() + "!" + command.user->getUsername() + "@localhost PART " + channelName + " :" + part_msg + "\r\n";
		for (std::vector<User*>::const_iterator it = chan->getMembers().begin(); it != chan->getMembers().end(); ++it)
		{
			send((*it)->getFd(), part_line.c_str(), part_line.length(), 0);
		}
		// Remove user from channel
		chan->removeMember(command.user);
	}
	else if (command.command == "WHO")
	{
		if (command.params.empty())
		{
			std::stringstream ss;
			ss << ERR_NEEDMOREPARAMS;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " WHO :Not enough parameters\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string channelName = command.params.front();
		if (channelName[0] != '#')
			channelName = "#" + channelName;
		Channel* chan = server.getChannelByName(channelName);
		if (!chan)
		{
			std::stringstream ss;
			ss << ERR_NOSUCHCHANNEL;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :No such channel\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
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
	else if (command.command == "KICK")
	{
		if (command.params.size() < 2)
		{
			std::stringstream ss;
			ss << ERR_NEEDMOREPARAMS;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " KICK :Not enough parameters\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string channelName = command.params[0];
		if (channelName[0] != '#')
			channelName = "#" + channelName;
		std::string targetNick = command.params[1];
		std::string reason = "Kicked";
		if (command.params.size() > 2)
		{
			reason = command.params[2];
			if (!reason.empty() && reason[0] == ':') reason = reason.substr(1);
		}
		Channel* chan = server.getChannelByName(channelName);
		if (!chan)
		{
			std::stringstream ss;
			ss << ERR_NOSUCHCHANNEL;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :No such channel\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		if (!chan->isOperator(command.user))
		{
			std::stringstream ss;
			ss << ERR_CHANOPRIVSNEEDED;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :You're not channel operator\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
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
			std::stringstream ss;
			ss << ERR_USERNOTINCHANNEL;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + targetNick + " " + channelName + " :They aren't on that channel\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		// Send KICK to all members
		std::string kick_line = ":" + command.user->getNickname() + "!" + command.user->getUsername() + "@localhost KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
		for (std::vector<User*>::const_iterator it = chan->getMembers().begin(); it != chan->getMembers().end(); ++it)
		{
			send((*it)->getFd(), kick_line.c_str(), kick_line.length(), 0);
		}
		// Remove user
		chan->removeMember(target);
	}
	else if (command.command == "INVITE")
	{
		if (command.params.size() < 2)
		{
			std::stringstream ss;
			ss << ERR_NEEDMOREPARAMS;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " INVITE :Not enough parameters\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string targetNick = command.params[0];
		std::string channelName = command.params[1];
		if (channelName[0] != '#')
			channelName = "#" + channelName;
		User* target = server.getClientFdByNickname(targetNick);
		if (!target)
		{
			std::stringstream ss;
			ss << ERR_NOSUCHNICK;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + targetNick + " :No such nick/channel\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		Channel* chan = server.getChannelByName(channelName);
		if (!chan)
		{
			std::stringstream ss;
			ss << ERR_NOSUCHCHANNEL;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :No such channel\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		if (!chan->isMember(command.user->getFd()))
		{
			std::stringstream ss;
			ss << ERR_NOTONCHANNEL;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :You're not on that channel\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
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
	}
	else if (command.command == "TOPIC")
	{
		if (command.params.empty())
		{
			std::stringstream ss;
			ss << ERR_NEEDMOREPARAMS;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " TOPIC :Not enough parameters\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string channelName = command.params[0];
		if (channelName[0] != '#')
			channelName = "#" + channelName;
		Channel* chan = server.getChannelByName(channelName);
		if (!chan)
		{
			std::stringstream ss;
			ss << ERR_NOSUCHCHANNEL;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :No such channel\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		if (!chan->isMember(command.user->getFd()))
		{
			std::stringstream ss;
			ss << ERR_NOTONCHANNEL;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :You're not on that channel\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
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
			// Set topic
			if (chan->isTopicProtected() && !chan->isOperator(command.user))
			{
				std::stringstream ss;
				ss << ERR_CHANOPRIVSNEEDED;
				std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + channelName + " :You're not channel operator\r\n";
				send(command.user->getFd(), err.c_str(), err.size(), 0);
				return;
			}
			std::string new_topic = command.params[1];
			if (!new_topic.empty() && new_topic[0] == ':') new_topic = new_topic.substr(1);
			chan->setTopic(new_topic);
			// Send TOPIC change to all members
			std::string topic_change = ":" + command.user->getNickname() + "!" + command.user->getUsername() + "@localhost TOPIC " + channelName + " :" + new_topic + "\r\n";
			for (std::vector<User*>::const_iterator it = chan->getMembers().begin(); it != chan->getMembers().end(); ++it)
			{
				send((*it)->getFd(), topic_change.c_str(), topic_change.length(), 0);
			}
		}
	}
	else if (command.command == "MODE")
	{
		if (command.params.empty())
		{
			std::stringstream ss;
			ss << ERR_NEEDMOREPARAMS;
			std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " MODE :Not enough parameters\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string target = command.params[0];
		if (target[0] == '#')
		{
			if (target[0] != '#')
				target = "#" + target;
			// Channel mode
			Channel* chan = server.getChannelByName(target);
			if (!chan)
			{
				std::stringstream ss;
				ss << ERR_NOSUCHCHANNEL;
				std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + target + " :No such channel\r\n";
				send(command.user->getFd(), err.c_str(), err.size(), 0);
				return;
			}
			if (!chan->isMember(command.user->getFd()))
			{
				std::stringstream ss;
				ss << ERR_NOTONCHANNEL;
				std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + target + " :You're not on that channel\r\n";
				send(command.user->getFd(), err.c_str(), err.size(), 0);
				return;
			}
			if (command.params.size() == 1)
			{
				// Show modes
				std::string modes = "+";
				if (chan->isInviteOnly()) modes += "i";
				if (chan->isTopicProtected()) modes += "t";
				std::string mode_line = ":" + std::string(ME) + " 324 " + command.user->getNickname() + " " + target + " " + modes + "\r\n";
				send(command.user->getFd(), mode_line.c_str(), mode_line.length(), 0);
				return;
			}
			// Set modes
			if (!chan->isOperator(command.user))
			{
				std::stringstream ss;
				ss << ERR_CHANOPRIVSNEEDED;
				std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + target + " :You're not channel operator\r\n";
				send(command.user->getFd(), err.c_str(), err.size(), 0);
				return;
			}
			std::string mode_str = command.params[1];
			char sign = mode_str[0];
			std::string mode = mode_str.substr(1);
			if (sign != '+' && sign != '-')
			{
				std::stringstream ss;
				ss << ERR_UNKNOWNMODE;
				std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + mode + " :is unknown mode char to me\r\n";
				send(command.user->getFd(), err.c_str(), err.size(), 0);
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
				if (command.params.size() < 3)
				{
					std::stringstream ss;
					ss << ERR_NEEDMOREPARAMS;
					std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " MODE :Not enough parameters\r\n";
					send(command.user->getFd(), err.c_str(), err.size(), 0);
					return;
				}
				std::string nick = command.params[2];
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
					std::stringstream ss;
					ss << ERR_USERNOTINCHANNEL;
					std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + nick + " " + target + " :They aren't on that channel\r\n";
					send(command.user->getFd(), err.c_str(), err.size(), 0);
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
				std::string err = ":" + std::string(ME) + " " + ss.str() + " " + command.user->getNickname() + " " + mode + " :is unknown mode char to me\r\n";
				send(command.user->getFd(), err.c_str(), err.size(), 0);
				return;
			}
			// Send MODE change to all members
			std::string mode_change = ":" + command.user->getNickname() + "!" + command.user->getUsername() + "@localhost MODE " + target + " " + mode_str;
			if (mode == "o") mode_change += " " + command.params[2];
			mode_change += "\r\n";
			for (std::vector<User*>::const_iterator it = chan->getMembers().begin(); it != chan->getMembers().end(); ++it)
			{
				send((*it)->getFd(), mode_change.c_str(), mode_change.length(), 0);
			}
		}
		else
		{
			// User mode - not implemented
			std::string err = ":" + std::string(ME) + " 501 " + command.user->getNickname() + " :Unknown MODE flag\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
		}
	}
	return;
}
/*
	if(message.find("KICK ") == 0)
	{
		
	}
	if(message.find("JOIN ") == 0)
	{
		channelName = message.substr(4);
		channelName.erase(0, channelName.find_first_not_of(WS));
		channelName.erase(channelName.find_last_not_of(WS) + 1);
		if (channelName.find_first_of(CHNAMEINVALID) != std::string::npos || (channelName[0] != REGCH && channelName[0] != LOCCH))
		{
			response = "Channel name not valid. Must not contain whitespace and comma and must start with `#` or `&`.\r\n";
			send(user->getFd(), response.c_str(), response.size(), 0);
		}
		else
			Server->joinChannel(channelName, user);
	}

	if(message.find("show") == 0)
	{
		std::cout << "Canales" << std::endl;
		for (std::map<std::string, Channel*>::const_iterator it = Server->getConstChannelList().begin(); it != Server->getConstChannelList().end(); ++it)
		{
			int i = it->second->getMembers().size();
			std::cout << it->first << " - " << i << std::endl;
		}
	}

}

*/