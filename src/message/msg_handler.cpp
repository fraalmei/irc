/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 11:38:21 by samartin          #+#    #+#             */
/*   Updated: 2026/01/30 17:22:01 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"
#include "../include/User.hpp"
#include "../include/validNames.hpp"
#include "../include/colors.hpp"
#include "../include/Commands.hpp"

int msg_handler::handle_buffer(std::string buffer, User *user)
{
	int msg_result = user->addToBuffer(buffer);
	if (msg_result == -1)
	{
		std::cout << "Received partial message without endline." << std::endl;
		std::cout << "User: " << user->getFd() << std::endl;
		std::cout << "Buffer: " << user->getBuffer() << std::endl;
		return 1;
	}
	else if (msg_result == 0)
	{
		std::cout << "Received empty message." << std::endl;
		std::cout << "User: " << user->getFd() << std::endl;
		std::cout << "Buffer: " << user->getBuffer() << std::endl;
		return 1;
	}
	return 0;
}

int	msg_handler::print_command(msg_handler::t_command command)
{
	std::cout << "Command: '" << command.command << "'" << std::endl;
	for (std::vector<std::string>::const_iterator it = command.params.begin(); it != command.params.end(); ++it)
	{
		std::cout << "Param: '" << *it << "'" << std::endl;
	}
	return 0;
}

msg_handler::t_command msg_handler::parse_msg(User* user)
{
	msg_handler::t_command	command;
	std::stringstream		ss;
    std::string				token;
	int						state = 0;

	command.actual_line = user->getBuffer().substr(0, user->getBuffer().find("\n"));
	user->removeFromBuffer(command.actual_line.length() + 1); // +1 to remove \n
	if (command.actual_line.find('\r') != std::string::npos)
		command.actual_line.erase(command.actual_line.length() - 1);

	if (command.actual_line.empty())
		command.user = NULL;
	else
	{
		command.user = user;
		ss.str(command.actual_line);
		ss.clear();
	// --- States ---
	// 0: Start/Waiting for Prefix or Command
	// 1: Reading Command
	// 2: Reading Middle Parameters
	// 3: Reading Trailing Parameter
	// IRC parsing logic is based in spaces and ':' character
		while (ss >> token)
		{
			if (state == 0)
			{
				// State 0: Start
				if (token[0] == ':')
				{
					// Transition to State 1 (Command)
					command.prefix = token.substr(1);
					state = 1; // Next token is the command
				}
				else
				{
					// Transition to State 2 (Middle Params)
					command.command = token;
					state = 2; // Next token is a param
				}
			}
			else if (state == 1)
			{
				// State 1: Reading command after source prefix
				// Transition to State 2 (Middle Params)
				command.command = token;
				state = 2;
			}
			else if (state == 2)
			{
				// State 2: Reading middle params
				if (token[0] == ':')
				{
					// Transition: ':' -> State 3 (Trailing Param)
					
					std::string trailing_part = token.substr(1); // All after the `:`
					std::string rest_of_line;
					
					// We need the current position in the stringstream to read the rest of the line
					// This part skips the stream-based reading to get all the remaining words.
					std::getline(ss, rest_of_line); 
					
					// The final param includes the rest of the line content.
					trailing_part += rest_of_line;
					command.params.push_back(trailing_part);
					state = 3; // End of logic, anything after `rest of line` would be rubish
					break; // Exit loop
				}
				else
				{
					// No transition from state 2 (Middle Params) (keeps reading params)
					command.params.push_back(token);
				}
			}
			// State 3 (Trailing Param) is the accept state and process no more tokens
		}
	}
	return command;
}

void msg_handler::execute_command(msg_handler::t_command command, Server &server)
{
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
		server.set_Authentication(command.user);
	}
	else if (!command.user->isAuthenticated() && command.command == "PASS")
	{
		if (handle_password(command, &server))
			return;
		server.set_Authentication(command.user);
	}
	else if (command.command == "CAP")
	{
		if (command.params.empty())
		{
			std::string err = std::string(":") + ME + " 461 CAP :Not enough parameters\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		command.user->setCapability(command.params[0]);
		// Respond to CAP negotiation
		std::string cap_response = "CAP * NAK\r\n";
		send(command.user->getFd(), cap_response.c_str(), cap_response.size(), 0);
	}
	else if (command.command == "JOIN")
	{
		Commands::commandJoin(command, server);
	}
	else if (command.command == "QUIT")
	{
		std::cout << "Client with fd " << command.user->getFd() << " sent QUIT command." << std::endl;
		server.ClearClients(command.user->getFd());
	}
	else if (command.command == "PRIVMSG")
	{
		Commands::commandPrivmsg(command, server);
	}
	else if (command.command == "PART")
	{
		Commands::commandPart(command, server);
	}
	else if (command.command == "WHO")
	{
		Commands::commandWho(command, server);
	}
	else if (command.command == "KICK")
	{
		Commands::commandKick(command, server);
	}
	else if (command.command == "INVITE")
	{
		Commands::commandInvite(command, server);
	}
	else if (command.command == "TOPIC")
	{
		Commands::commandTopic(command, server);
	}
	else if (command.command == "MODE")
	{
		Commands::commandMode(command, server);
	}
	return;
}
