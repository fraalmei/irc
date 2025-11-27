/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:26:30 by p                 #+#    #+#             */
/*   Updated: 2025/11/27 02:26:00 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"
#include "../include/User.hpp"
#include "../include/validNames.hpp"
#include "../include/colors.hpp"

int	msg_handler::handle_login_parse(User *user, Server *server)
{
	std::string buffer = server->handle_client_message(user); // receive message into user buffer
	parse_user(buffer, user, server);
    return 0;
}

int	msg_handler::parse_user(std::string buffer, User *user, Server *server)
{
	// PASO 1: Validar contraseña
	if (!user->isPasswdCorrect())
	{
		int result = handle_password(buffer, user, server);
		if (result == 0)  // Contraseña correcta
		{
			user->setPasswdCorrect(true);  // Establecer flag de contraseña correcta
			std::string prompt = "Envía tu nickname\n";
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
		handle_nickname(buffer, user);
		std::string prompt = "Envía tu username\n";
		send(user->getFd(), prompt.c_str(), prompt.size(), 0);
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nickname recibido, esperando username." << std::endl;
		return 0;  // Espera siguiente mensaje (username)
	}
	
	// PASO 3: Obtener username
	if (user->getUsername().empty())
	{
		handle_username(buffer, user);
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Usuario completamente autenticado." << std::endl;
		return 0;  // Ya está completo
	}

	return 0;
}

int	msg_handler::handle_password(std::string buffer, User *user, Server *server)
{
	(void)user; // evitar warning de variable no usada
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer recibido: '" << buffer << "'." << std::endl;
	if (buffer.empty() || buffer[0] == '\0')
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer erroneo." << std::endl;
		return 1;
	}
	std::string passwd(buffer);
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

int	msg_handler::handle_nickname(std::string buffer, User *user)
{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer recibido: '" << buffer << "'." << std::endl;
	if (buffer.empty() || buffer[0] == '\0')
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer erroneo." << std::endl;
		return 1;
	}
	
	// Trimmar espacios y saltos de línea (izquierda)
	buffer.erase(0, buffer.find_first_not_of(" \n\r\t"));
	// Trimmar espacios y saltos de línea (derecha)
	buffer.erase(buffer.find_last_not_of(" \n\r\t") + 1);
	
	if (buffer.empty())
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nickname vacío después de trimmar." << std::endl;
		return 1;
	}
	
	user->setNickname(buffer);
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nickname establecido: '" << buffer << "'." << std::endl;
	return 0;
}

int	msg_handler::handle_username(std::string buffer, User *user)
{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer recibido: '" << buffer << "'." << std::endl;
	if (buffer.empty() || buffer[0] == '\0')
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer erroneo." << std::endl;
		return 1;
	}
	
	// Trimmar espacios y saltos de línea (izquierda)
	buffer.erase(0, buffer.find_first_not_of(" \n\r\t"));
	// Trimmar espacios y saltos de línea (derecha)
	buffer.erase(buffer.find_last_not_of(" \n\r\t") + 1);
	
	if (buffer.empty())
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Username vacío después de trimmar." << std::endl;
		return 1;
	}
	
	user->setUsername(buffer);
	// Mark user as authenticated and welcome
	user->setAuthenticated(true);
	std::string welcome = "Usuario completo. Bienvenido\n";
	send(user->getFd(), welcome.c_str(), welcome.size(), 0);
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Username establecido: '" << buffer << "'." << std::endl;
	return 0;
}
