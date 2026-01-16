/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 11:38:21 by p                 #+#    #+#             */
/*   Updated: 2026/01/16 16:01:56 by samartin         ###   ########.fr       */
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
	for (std::list<std::string>::const_iterator it = command.params.begin(); it != command.params.end(); ++it)
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
		if (command.user->getUsername() != "")
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario ya autenticado con username: " << command.user->getUsername() << std::endl;
		}
		if (command.params.empty())
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " ERROR: No se han proporcionado parámetros para el comando USER." << std::endl;
			std::string err = std::string(":") + ME + " 461 USER :Not enough parameters\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		command.user->setUsername(command.params.front().substr(0, command.params.front().find(' ')));
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Username set to: '" << command.user->getUsername() << "'" << std::endl;
		server.set_Authentication(command.user);
	}
	else if (!command.user->isAuthenticated() && command.command == "NICK")
	{
		if (command.user->getNickname() != "")
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario ya autenticado con nickname: " << command.user->getNickname() << std::endl;
		}
		if (command.params.empty())
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " ERROR: No se han proporcionado parámetros para el comando NICK." << std::endl;
			std::string err = std::string(":") + ME + " 431 NICK :No nickname given\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		command.user->setNickname(command.params.front());
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nickname set to: '" << command.user->getNickname() << "'" << std::endl;
		server.set_Authentication(command.user);
	}
	else if (!command.user->isAuthenticated() && command.command == "PASS")
	{
		if (command.params.empty())
		{
			std::string err = std::string(":") + ME + " 461 PASS :Not enough parameters\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string pass = command.params.front();
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "PASS: '" << pass << "'" << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Password: '" << server.get_password() << "'" << std::endl;
		if (pass.compare(server.get_password()) == 0)
		{
			command.user->setPasswdCorrect(true);
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Password set to true." << std::endl;
		}
		else
		{
			std::string err = std::string(":") + ME + " 464 :Password incorrect\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Password incorrect, disconnecting." << std::endl;
			server.ClearClients(command.user->getFd());
			return;
		}
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Password : " << command.user->isPasswdCorrect() << std::endl;
		server.set_Authentication(command.user);
	}
	else if (command.command == "CAP")
	{
		command.user->setCapability(command.params.front().substr(0, command.params.front().find(' ')));
		command.user->setVersion(std::atoi(command.params.front().substr(command.params.front().find(' ') + 1).c_str()));
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Capability set to: " << command.user->getCapability() << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Version set to: " << command.user->getVersion() << std::endl;
	}
	else if (command.command == "/join" || command.command == "JOIN")
	{
		if (command.params.empty())
		{
			std::string err = std::string(":") + ME + " 461 JOIN :Not enough parameters\r\n";
			send(command.user->getFd(), err.c_str(), err.size(), 0);
			return;
		}
		std::string channelName = command.params.front();
		
		server.joinChannel(channelName, command.user);
	}
	else if (command.command == "QUIT")
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Client with fd " << command.user->getFd() << " sent QUIT command." << std::endl;
		server.ClearClients(command.user->getFd());
	}
	else
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Comando no implementado: " << command.user->getBuffer() << std::endl;
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