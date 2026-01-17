/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 11:38:21 by p                 #+#    #+#             */
/*   Updated: 2026/01/17 21:09:38 by p                ###   ########.fr       */
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
		command.user->setCapability(command.params.front().substr(0, command.params.front().find(' ')));
		command.user->setVersion(std::atoi(command.params.front().substr(command.params.front().find(' ') + 1).c_str()));
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Capability set to: " << command.user->getCapability() << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Version set to: " << command.user->getVersion() << std::endl;
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
		
		joinChannel(channelName, command.user, server);
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