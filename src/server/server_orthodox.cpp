/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_orthodox.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:05:36 by p                 #+#    #+#             */
/*   Updated: 2025/11/26 23:37:23 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Definition of static member
bool Server::_signal = false;

// Constructors
Server::Server() : _server_fd(-1), _fd_max(0)
{
	init_server_socket();
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Server created." << std::endl;
}

Server::Server(char *password, int port) : _server_fd(-1), _fd_max(0), _port(port), _password(password)
{
	init_server_socket();
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Server created with password and port." << std::endl;
}

Server::Server(const Server &copy)
{
	this->set_fd_max(copy._fd_max);
	this->set_master_set(copy._master_set);
	this->set_server_fd(copy._fd_max);
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Server copied." << std::endl;
}

// Destructor
Server::~Server(void)
{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Server destroyed." << std::endl;
}

// Operators
Server & Server::operator=(const Server &assign)
{
	this->set_fd_max(assign._fd_max);
	this->set_master_set(assign._master_set);
	this->set_server_fd(assign._fd_max);
	return *this;
}
