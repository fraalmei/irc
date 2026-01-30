/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 11:38:42 by saamrtin          #+#    #+#             */
/*   Updated: 2026/01/30 10:02:41 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "msg_handler.hpp"
#include "IrcResponses.hpp"

int	Server::closeClientFd(int fd)
{
	// Close the client file descriptor and remove it from the clients map
	// Cerrar el descriptor de archivo del cliente y eliminarlo del mapa de clientes
	std::map<int, User*>::iterator it = getClientList().find(fd);
	if (it != getClientList().end())
	{
		delete it->second; // Free the User object
		getClientList().erase(it); // Remove from the map
		close(fd); // Close the file descriptor
		std::cout << "Closed client fd " << fd << "." << std::endl;
		return 0; // Success
	}
	std::cout << "Client fd " << fd << " not found." << std::endl;
	return -1; // Failure
}

int Server::set_Authentication(User *client)
{
	std::cout << "Checking authentication." << std::endl;
	std::cout << "Client Nickname: " << client->getNickname() << "." << std::endl;
	std::cout << "Client Username: " << client->getUsername() << "." << std::endl;
	std::cout << "Client Password: " << client->isPasswdCorrect() << "." << std::endl;
	
	if (client->getNickname() != "" && client->getUsername() != "" && client->isPasswdCorrect())
	{
		std::cout << "Authenticated." << std::endl;
		
		// Send all welcome messages
		IrcResponses::sendWelcomeMessages(client);
		
		client->setAuthenticated(true);
		return 1;
	}
	else
	{
		std::cout << "Not authenticated yet." << std::endl;
		return 0;
	}
}
