/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 11:38:42 by p                 #+#    #+#             */
/*   Updated: 2026/01/18 15:00:51 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "msg_handler.hpp"

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
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Closed client fd " << fd << "." << std::endl;
		return 0; // Success
	}
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Client fd " << fd << " not found." << std::endl;
	return -1; // Failure
}

int Server::set_Authentication(User *client)
	{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Cheking autentication." << std::endl;
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Client Nickname: " << client->getNickname() << "." << std::endl;
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Client Username: " << client->getUsername() << "." << std::endl;
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Client Password: " << client->isPasswdCorrect() << "." << std::endl;
	if (client->getNickname() != "" && client->getUsername() != "" && client->isPasswdCorrect())
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Autenticated." << std::endl;
		// Helper to convert int to string
		std::stringstream ss;
		ss << RPL_WELCOME;
		std::string code = ss.str();
		ss.str(""); ss << RPL_YOURHOST;
		std::string code2 = ss.str();
		ss.str(""); ss << RPL_CREATED;
		std::string code3 = ss.str();
		ss.str(""); ss << RPL_MYINFO;
		std::string code4 = ss.str();
		
		// Send welcome messages
		std::string welcome = ":" + std::string(ME) + " " + code + " " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@localhost\r\n";
		send(client->getFd(), welcome.c_str(), welcome.size(), 0);
		
		std::string yourhost = ":" + std::string(ME) + " " + code2 + " " + client->getNickname() + " :Your host is " + std::string(ME) + ", running version 1.0\r\n";
		send(client->getFd(), yourhost.c_str(), yourhost.size(), 0);
		
		std::string created = ":" + std::string(ME) + " " + code3 + " " + client->getNickname() + " :This server was created 2026-01-17\r\n";
		send(client->getFd(), created.c_str(), created.size(), 0);
		
		std::string myinfo = ":" + std::string(ME) + " " + code4 + " " + client->getNickname() + " " + std::string(ME) + " 1.0 o o\r\n";
		send(client->getFd(), myinfo.c_str(), myinfo.size(), 0);
		
		client->setAuthenticated(true);
		return 1;
	}
	else
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Not autenticated yet." << std::endl;
		return 0;
	}
}
