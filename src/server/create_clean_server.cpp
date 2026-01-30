/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_clean_server.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 09:08:51 by fraalmei          #+#    #+#             */
/*   Updated: 2026/01/30 10:03:58 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"

/// @brief server socket initializer function
void	Server::init_server_socket()
{
	int	en = 1;

	// server address struct
	struct sockaddr_in	add;
	add.sin_family = AF_INET;	// set the address family to ipv4
	add.sin_addr.s_addr = INADDR_ANY;	// set the address to any local machine address
	add.sin_port = htons( this->_port );	// convert the port to network byte

	set_server_fd( socket( AF_INET, SOCK_STREAM, 0 ));	// create the server socket
	if(get_server_fd() == -1)	// check if the socket is created
	{
		std::cout << "Error creating socket." << std::endl;
		exit(1);
	}

	if(setsockopt(get_server_fd(), SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)	// set the socket option (SO_REUSEADDR) to reuse the address
	{
		std::cout << "Error in setsockopt()." << std::endl;
		close ( get_server_fd() );
		exit (1);
	}
	if ( fcntl( get_server_fd(), F_SETFL, O_NONBLOCK ) == -1 )	// set the socket option (O_NONBLOCK) for non-blocking socket
	{
		std::cout << "Error in fcntl()." << std::endl;
		close ( get_server_fd() );
		exit (1);
	}
	if ( bind( get_server_fd(), (struct sockaddr *)&add, sizeof(add)) == -1 )	// bind the socket to the address
	{
		std::cout << "Error in bind()." << std::endl;
		close( get_server_fd() );
		exit(1);
	}
	if ( listen( get_server_fd(), BACKLOG ) == -1 )	// listen for incoming connections and making the socket a passive socket
	{
		std::cout << "Error in listen()." << std::endl;
		close ( get_server_fd() );
		exit (1);
	}
	
	struct pollfd		NewPoll;
	NewPoll.fd = get_server_fd();	// add the server socket to the pollfd
	NewPoll.events = POLLIN;		// set the event to POLLIN for reading data
	NewPoll.revents = 0;			// set the revents to 0
	_fds.push_back(NewPoll);		// add the server socket to the pollfd

	std::cout << "Server started on port " << _port << std::endl;
	std::cout << "Server started with password: '" << _password << "'" << std::endl;

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
