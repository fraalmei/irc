/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:26:30 by samartin          #+#    #+#             */
/*   Updated: 2026/01/30 11:39:27 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"
#include "../include/User.hpp"
#include "../include/validNames.hpp"
#include "../include/colors.hpp"

int	msg_handler::handle_password(msg_handler::t_command command, Server *server)
{
	if (command.params.empty())
	{
		std::string err = std::string(":") + ME + " 461 PASS :Not enough parameters\r\n";
		send(command.user->getFd(), err.c_str(), err.size(), 0);
		return 1;
	}
	std::string pass = command.params[0];
	if (pass.compare(server->get_password()) == 0)
		command.user->setPasswdCorrect(true);
	else
	{
		std::string err = std::string(":") + ME + " 464 :Password incorrect\r\n";
		send(command.user->getFd(), err.c_str(), err.size(), 0);
		std::cout << CRED << " Password incorrect, disconnecting user." << CRST << std::endl;
		server->ClearClients(command.user->getFd());
		return 1;
	}
	return 0;
}

int	msg_handler::handle_nickname(msg_handler::t_command command)
{
	if (command.params.empty())
	{
		std::string err = std::string(":") + ME + " 431 NICK :No nickname given\r\n";
		send(command.user->getFd(), err.c_str(), err.size(), 0);
		return 1;
	}
	command.user->setNickname(command.params[0]);
	return 0;
}


int	msg_handler::handle_username(msg_handler::t_command command)
{
	// 
	if (command.user->getUsername() != "")
		return 1;
	if (command.params.empty())
	{
		std::string err = std::string(":") + ME + " 461 USER :Not enough parameters\r\n";
		send(command.user->getFd(), err.c_str(), err.size(), 0);
		return 1;
	}
	command.user->setUsername(command.params[0]);
	std::cout << " Username set to: '" << command.user->getUsername() << "'" << std::endl;
	return 0;
}
