/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:26:30 by p                 #+#    #+#             */
/*   Updated: 2026/01/16 15:57:07 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"
#include "../include/User.hpp"
#include "../include/validNames.hpp"
#include "../include/colors.hpp"

int	msg_handler::aunthenticateUser(User *user, Server *server)
{
	if (server->handle_client_message(user).empty()) // receive message into user buffer
		return 1;
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
	// PASO 1: Validar contraseña
	if (!user->isPasswdCorrect())
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
	}

	return 0;
}

int	msg_handler::handle_password(User *user, Server *server)
{
	(void)user; // evitar warning de variable no usada
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer recibido: '" << user->getBuffer() << "'." << std::endl;
	if (user->getBuffer().empty() || user->getBuffer()[0] == '\0')
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer erroneo." << std::endl;
		return 1;
	}
	std::string passwd(user->getBuffer());
	passwd.erase(passwd.find_last_not_of(" \n\r\t") + 1);  // Trim derecha

	if (server->get_password() != passwd)
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Contraseña incorrecta." << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Servidor espera: '" << server->get_password() << "'." << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario envió: '" << passwd << "'." << std::endl;
		return 2;
	}
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Contraseña validada correctamente." << std::endl;
	return 0;
}

int	msg_handler::handle_nickname(User *user)
{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer recibido: '" << user->getBuffer() << "'." << std::endl;
	if (user->getBuffer().empty() || user->getBuffer()[0] == '\0')
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer erroneo." << std::endl;
		return 1;
	}

	// Trimmar espacios y saltos de línea (izquierda)
	user->getBuffer().erase(0, user->getBuffer().find_first_not_of(" \n\r\t"));
	// Trimmar espacios y saltos de línea (derecha)
	user->getBuffer().erase(user->getBuffer().find_last_not_of(" \n\r\t") + 1);
	
	if (user->getBuffer().empty())
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nickname vacío después de trimmar." << std::endl;
		return 1;
	}

	user->setNickname(user->getBuffer());
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nickname establecido: '" << user->getBuffer() << "'." << std::endl;
	return 0;
}

int	msg_handler::handle_username(User *user)
{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer recibido: '" << user->getBuffer() << "'." << std::endl;
	if (user->getBuffer().empty() || user->getBuffer()[0] == '\0')
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer erroneo." << std::endl;
		return 1;
	}
	
	// Trimmar espacios y saltos de línea (izquierda)
	user->getBuffer().erase(0, user->getBuffer().find_first_not_of(" \n\r\t"));
	// Trimmar espacios y saltos de línea (derecha)
	user->getBuffer().erase(user->getBuffer().find_last_not_of(" \n\r\t") + 1);
	
	if (user->getBuffer().empty())
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Username vacío después de trimmar." << std::endl;
		return 1;
	}

	user->setUsername(user->getBuffer());
	// Mark user as authenticated and welcome
	user->setAuthenticated(true);
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Username establecido: '" << user->getBuffer() << "'." << std::endl;
	return 0;
}
