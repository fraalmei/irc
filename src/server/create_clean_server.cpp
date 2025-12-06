/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_clean_server.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 09:08:51 by p                 #+#    #+#             */
/*   Updated: 2025/12/06 11:22:32 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/// @brief server socket initializer function
/// función para inicializar el socket del servidor
void	Server::init_server_socket()
{
	int					en = 1;

	// server address struct
	struct sockaddr_in	add;
	add.sin_family = AF_INET;	// set the address family to ipv4
	add.sin_addr.s_addr = INADDR_ANY;	// set the address to any local machine address
	add.sin_port = htons( this->_port );	// convert the port to network byte

	set_server_fd( socket( AF_INET, SOCK_STREAM, 0 ));	// create the server socket
	if(get_server_fd() == -1)	// check if the socket is created
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error al crear el socket." << std::endl;
		exit(1);
	}
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Servidor iniciado en el puerto " << _port << std::endl;

	if(setsockopt(get_server_fd(), SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)	// set the socket option (SO_REUSEADDR) to reuse the address
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en setsockopt()." << std::endl;
		close ( get_server_fd() );
		exit (1);
	}
	if ( fcntl( get_server_fd(), F_SETFL, O_NONBLOCK ) == -1 )	// set the socket option (O_NONBLOCK) for non-blocking socket
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en fcntl()." << std::endl;
		close ( get_server_fd() );
		exit (1);
	}
	if ( bind( get_server_fd(), (struct sockaddr *)&add, sizeof(add)) == -1 )	// bind the socket to the address
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en bind()." << std::endl;
		close( get_server_fd() );
		exit(1);
	}
	if ( listen( get_server_fd(), BACKLOG ) == -1 )	// listen for incoming connections and making the socket a passive socket
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en listen()." << std::endl;
		close ( get_server_fd() );
		exit (1);
	}
	
	struct pollfd		NewPoll;
	NewPoll.fd = get_server_fd();		// add the server socket to the pollfd
	NewPoll.events = POLLIN;		// set the event to POLLIN for reading data
	NewPoll.revents = 0;			// set the revents to 0
	_fds.push_back(NewPoll);		// add the server socket to the pollfd

	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Servidor iniciado en el puerto " << _port << std::endl;
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Servidor iniciado con contraseña: '" << _password << "'" << std::endl;

}

void	Server::CloseFds()
{
    const std::string quit_msg = "ERROR :Server shutting down\r\n";

	for (std::map<int, User*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		std::cout << CRED << "Client <" << it->first << "> Disconnected" << CRST << std::endl;
		send(it->first, quit_msg.c_str(), quit_msg.size(), 0);
		close(it->first);
		delete it->second;
	}
	// clear the clients map
	_clients.clear();
	
	// send quit message to all clients before closing
	_fds.clear();

	if (get_server_fd() >= 0) // close the server socket
	{
		std::cout << CRED << "Server <" << get_server_fd() << "> Disconnected" << CRST << std::endl;
		close(get_server_fd());
		set_server_fd(-1);
    }

}

void Server::ClearClients(int fd)	// clear the clients
{
	for(size_t i = 0; i < _fds.size(); i++)
	{
		if (_fds[i].fd == fd)
		{
			_fds.erase(_fds.begin() + i);
			break;
		}
	}
	// remove and delete client from the client map
	std::map<int, User*>::iterator it = _clients.find(fd);
	if (it != _clients.end())
	{
		delete it->second;
		_clients.erase(it);
	}

}
