/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:26:30 by p                 #+#    #+#             */
/*   Updated: 2026/01/17 16:02:22 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"
#include "../include/User.hpp"
#include "../include/validNames.hpp"
#include "../include/colors.hpp"

int	msg_handler::aunthenticateUser(User *user, Server *server)
{
	server->handle_client_message(user); // receive message into user buffer
	authenticate(user, server);
	user->clearBuffer();
	return 0;
}

// Rehacer: La mayoría de los clientes enviarán los mensajes de login en paquete y podría ser que en distinto orden.
// En vez de bloquear pidiendo paso por paso, según haya mensajes completos en el buffer, pasarlos a command con el
// msg_handler::parse_msg() y comprobar cada uno si va cumpliendo con lo que va faltando por autentificar. Normalmente
// habrá uno con el comando "CAP" que podemos ignorar de momento. 
int	msg_handler::authenticate(User *user, Server *server)
{
	(void) user;
	(void) server;
	// PASO 1: Validar contraseña
	/*if (!user->isPasswdCorrect())
	{
		int result = handle_password(user, server);
		if (result == 0)  // Contraseña correcta
		{
			user->setPasswdCorrect(true);  // Establecer flag de contraseña correcta
			std::string prompt = "Envía tu nickname\r\n";
			send(user->getFd(), prompt.c_str(), prompt.size(), 0);
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Contraseña correcta, esperando nickname." << std::endl;
			return 0;  // Espera siguiente mensaje (nickname)
		}
		else
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Contraseña incorrecta, esperando reintento." << std::endl;
			return 0;  // Espera reintento de contraseña
		}
	}
	
	// PASO 2: Obtener nickname
	if (user->getNickname().empty())
	{
		handle_nickname(user);
		std::string prompt = "Envía tu username\r\n";
		send(user->getFd(), prompt.c_str(), prompt.size(), 0);
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nickname recibido, esperando username." << std::endl;
		return 0;  // Espera siguiente mensaje (username)
	}
	
	// PASO 3: Obtener username
	if (user->getUsername().empty())
	{
		handle_username(user);
		std::string welcome = "Usuario completo. Bienvenido\r\n";
		send(user->getFd(), welcome.c_str(), welcome.size(), 0);
		std::string prompt = std::string(":") + ME + " CAP * LS :\r\n";
		send(user->getFd(), prompt.c_str(), prompt.size(), 0);
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario completamente autenticado." << std::endl;
		return 0;  // Ya está completo
	}*/
	return 0;
}

int	msg_handler::handle_password(msg_handler::t_command command, Server *server)
{
	if (command.params.empty())
	{
		std::string err = std::string(":") + ME + " 461 PASS :Not enough parameters\r\n";
		send(command.user->getFd(), err.c_str(), err.size(), 0);
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " ERROR: No se han proporcionado parámetros para el comando PASS." << std::endl;
		return 1;
	}
	std::string pass = command.params.front();
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "PASS: '" << pass << "'" << std::endl;
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Password: '" << server->get_password() << "'" << std::endl;
	if (pass.compare(server->get_password()) == 0)
	{
		command.user->setPasswdCorrect(true);
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Password set to true." << std::endl;
	}
	else
	{
		std::string err = std::string(":") + ME + " 464 :Password incorrect\r\n";
		send(command.user->getFd(), err.c_str(), err.size(), 0);
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Password incorrect, disconnecting." << std::endl;
		server->ClearClients(command.user->getFd());
		return 1;
	}
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Password : " << command.user->isPasswdCorrect() << std::endl;
	return 0;
}

int	msg_handler::handle_nickname(msg_handler::t_command command)
{
	if (command.user->getNickname() != "")
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario ya autenticado con nickname: " << command.user->getNickname() << std::endl;
		return 1;
	}
	if (command.params.empty())
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " ERROR: No se han proporcionado parámetros para el comando NICK." << std::endl;
		std::string err = std::string(":") + ME + " 431 NICK :No nickname given\r\n";
		send(command.user->getFd(), err.c_str(), err.size(), 0);
		return 1;
	}
	command.user->setNickname(command.params.front());
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nickname set to: '" << command.user->getNickname() << "'" << std::endl;
	return 0;
}


int	msg_handler::handle_username(msg_handler::t_command command)
{
	// 
	if (command.user->getUsername() != "")
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario ya autenticado con username: " << command.user->getUsername() << std::endl;
		return 1;
	}
	if (command.params.empty())
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " ERROR: No se han proporcionado parámetros para el comando USER." << std::endl;
		std::string err = std::string(":") + ME + " 461 USER :Not enough parameters\r\n";
		send(command.user->getFd(), err.c_str(), err.size(), 0);
		return 1;
	}
	command.user->setUsername(command.params.front().substr(0, command.params.front().find(' ')));
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Username set to: '" << command.user->getUsername() << "'" << std::endl;
	return 0;
}
