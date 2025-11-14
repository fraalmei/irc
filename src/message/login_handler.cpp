/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:26:30 by p                 #+#    #+#             */
/*   Updated: 2025/11/13 17:53:27 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"
#include "../include/User.hpp"
#include "../include/validNames.hpp"

int	msg_handler::handle_login_parse(User *user, Server *server)
{
    std::string	buffer;
	std::string	response;
	int			nbytes = recv( user->getFd(), &buffer[0], BUFFER_SIZE - 1, 0 ); // data receiv

	buffer.resize(BUFFER_SIZE);
	if (user->getNickname() != "")
		std::cout << "Se ha recibido mensaje de " << user->getNickname() << ": '" << buffer <<  "' con " << buffer.size() << " chars and " << nbytes << " bytes." << std::endl;
	if (nbytes < 0)
    {
        perror("recv");		// mostrar error exacto
        close( user->getFd() );
        FD_CLR( user->getFd(), server->get_master_set_ptr() );
    }
    else if(nbytes == 0)
    {
        // if an error ocurr or the client desconetion
        std::cout << "Socket " << user->getNickname() << " disconected for the user." << std::endl;

        // close the user socket and remove it from the set
        close( user->getFd() );
        FD_CLR( user->getFd(), server->get_master_set_ptr() );
    }
    else
    {
        buffer.resize(nbytes);                      // ajustar al tamaño real recibido
        buffer.push_back('\0');                      // asegurar terminador si lo necesita el código siguiente

        std::cout << "Se ha recibido mensaje de " << user->getNickname() << ": '" << buffer <<  "' con " << buffer.size() << " chars and " << nbytes << " bytes." << std::endl;
        std::cout << "Buffer recibido de " << user->getFd() << ": " << buffer;
        std::string	message = buffer.substr(0, buffer.size() - 3); // remove \n\r
        std::cout << "Mensaje recibido de " << user->getFd() << ": " << message;
						
        parse_user(message, user, server);
    }
    return 0;
}

int	msg_handler::parse_user(std::string buffer, User *user, Server *server)
{
	if (!user->isPasswdCorrect())
	{
		handle_password(buffer, user, server);
	}

	if (user->getNickname().empty() && user->getUsername().empty())
	{
		handle_nickname(buffer.substr(0, buffer.find(':')), user);
		handle_username(buffer.substr(buffer.find(':'), buffer.find('\n')), user);
	}

	return 0;
}

int	msg_handler::handle_password(std::string buffer, User *user, Server *server)
{
	std::cout << "Dentro de handle_password con buffer '" << buffer << "'." << std::endl;
	if (buffer.empty() || buffer[0] == '\0')
	{
		std::cout << "Buffer erroneo." << std::endl;
		return 1;
	}
	if (server->get_password() != buffer)
	{
		std::cout << "Invalid password from user " << user->getFd() << "." << std::endl;
		std::cout << "Server password '" << server->get_password() << "'." << std::endl;
		std::cout << "User password '" << buffer << "'." << std::endl;
		return 2;
	}
	user->setAuthenticated(true);
	std::string welcome = "Bienvenido. Por favor, envía tu nickname:username\n";
	send(user->getFd(), welcome.c_str(), welcome.size(), 0);
	std::cout << "Handling password, return int." << std::endl;
	return 0;
}

int	msg_handler::handle_nickname(std::string buffer, User *user)
{
	std::cout << "Dentro de handle_nickname." << std::endl;
	if (buffer.empty() || buffer[0] == '\0')
		return 1;
	user->setNickname(std::string(buffer));
	std::cout << "Handling nickname, return int." << std::endl;
	return 0;
}

int	msg_handler::handle_username(std::string buffer, User *user)
{
	std::cout << "Dentro de handle_username." << std::endl;
	if (buffer.empty() || buffer[0] == '\0')
		return 1;
	user->setUsername(std::string(buffer));
	std::string welcome = "Usuario completo. Bienvenido\n";
	send(user->getFd(), welcome.c_str(), welcome.size(), 0);
	std::cout << "Handling username, return int." << std::endl;
	return 0;
}
