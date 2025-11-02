/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_orthodox.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraalmei <fraalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:05:36 by p                 #+#    #+#             */
/*   Updated: 2025/11/02 15:01:54 by fraalmei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Constructors
Server::Server() : _server_fd(-1), _fd_max(0)
{
	init_server_socket();
	std::cout << "Server created." << std::endl;
}

Server::Server(char *password, int port) : _server_fd(-1), _fd_max(0), _port(port), _password(password)
{
	init_server_socket();
	std::cout << "Server created with password and port." << std::endl;
}

Server::Server(const Server &copy)
{
	this->set_fd_max(copy._fd_max);
	this->set_master_set(copy._master_set);
	this->set_read_fds(copy._read_fds);
	this->set_server_fd(copy._fd_max);
	std::cout << "Server copied." << std::endl;
}

// Destructor
Server::~Server(void)
{
	// close the server
	// cierra el servidor
	close(get_server_fd());

	// iter all the clients closing the conexion
	// itera por todos los clientes cerrando la conexión

	for(std::map<int, User*>::iterator it = _client_list.begin(); it != _client_list.end(); ++it )
		delete it->second; // delete each client
	_client_list.clear(); // clear the map of clients

	for(std::map<int, User*>::iterator it = _client_list.begin(); it != _client_list.end(); ++it)
		delete it->second; // delete each channel
	_channel_list.clear(); // clear the map of channels

	std::cout << "Server destroyed." << std::endl;
}

// Operators
Server & Server::operator=(const Server &assign)
{
	this->set_fd_max(assign._fd_max);
	this->set_master_set(assign._master_set);
	this->set_read_fds(assign._read_fds);
	this->set_server_fd(assign._fd_max);
	return *this;
}
