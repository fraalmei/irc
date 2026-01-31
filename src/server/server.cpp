/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:53:11 by samartin          #+#    #+#             */
/*   Updated: 2026/01/30 10:08:19 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"

/// @brief handle a new incoming conection
void	Server::handle_new_connection()
{
	struct sockaddr_in	client_addr;
	socklen_t	addrlen = sizeof( client_addr );

	// Accept the new conexion and get its socket
	int			new_fd = accept( get_server_fd(), ( sockaddr* )&client_addr, &addrlen );
	if (new_fd == -1)
	{
		std::cout << " Error in accept()." << std::endl;
		return;
	}
	if (fcntl( new_fd, F_SETFL, O_NONBLOCK ) == -1 )	// set the socket option (O_NONBLOCK) for non-blocking socket
	{
		std::cout << " Error in fcntl()." << std::endl;
		close ( new_fd );
		return;
	}

	// add the new client socket to the poll list so we can monitor it
	struct pollfd clientPoll;
	clientPoll.fd = new_fd;
	clientPoll.events = POLLIN;
	clientPoll.revents = 0;

	User* new_client = new User(new_fd);                     // create a new user object
	new_client->setIpAdd(inet_ntoa( client_addr.sin_addr )); // set the client IP address

	// register new client in the clients map
	getClientList()[new_fd] = new_client;

	// register new client in the pollfd list
	_fds.push_back(clientPoll);

	std::cout << "New conection from " << inet_ntoa( client_addr.sin_addr ) << " in socket " << new_fd << std::endl;
}

/// @brief Receives a message and filters it through the buffer handler, which saves it in the User object until complete
/// @param client_fd
void	Server::handle_client_message(User *user)
{
	char	initbuffer[BUFFER_SIZE];			// buffer to receive data
	memset(initbuffer, 0, sizeof(initbuffer));	// clear the buffer
	ssize_t	nbytes = recv( user->getFd(), initbuffer, BUFFER_SIZE - 1, 0 ); // data receive

	if (nbytes < 0)
	{
		// Error in recv
		// Check if it's a non-blocking error
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// No data available, not an error - just return without cleaning
			std::cout << "No data available (EAGAIN), continuing." << std::endl;
		}
		else
		{
			// Real error or disconnection
			std::cout << "Error in recv: " << strerror(errno) << std::endl;
			ClearClients(user->getFd());
		}
	}
	else if (nbytes == 0)
	{
		// Client disconnected
		std::cout << "Client disconnected (nbytes == 0)." << std::endl;
		ClearClients(user->getFd());
	}

	std::string buffer(initbuffer);	// convert to std::string

	user->addToBuffer(buffer);
	while (!user->getBuffer().empty() && user->getBuffer().find("\r\n") != std::string::npos) {
		msg_handler::t_command parsed_command = msg_handler::parse_msg(user);
		if (parsed_command.user != NULL) {
			msg_handler::execute_command(parsed_command, *this);
			// Check if user was disconnected during command execution
			if (_clients.find(user->getFd()) == _clients.end()) {
				break;
			}
		}
	}
}

/// @brief Signal handler for graceful shutdown
void	Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	set_signal(true);	// set the static boolean to true to stop the server
}

///	master loop
void	Server::run()
{
	while (!get_signal())
	{
		// wait for activity on any of the sockets
		if ((poll(&_fds[0], _fds.size(), -1) == -1) && !get_signal())
		{
			std::cout << "poll() error." << std::endl;
			exit(1);
		}

		// check all fds in the poll list
		for(size_t i = 0; i < _fds.size(); i++)
		{
			// check if there is activity on socket i
			if (_fds[i].revents & POLLIN)	// 
			{
				// if the activity is on the server socket, it is a new connection
				if (_fds[i].fd == get_server_fd())		
				{
					// new incoming conection
					handle_new_connection();
				}
				else
				{
					// message from a client
					User *user = getClientByFd(_fds[i].fd);
					if (user == NULL)
					{
						// This should not happen, but just in case
						std::cout << "Client with fd " << _fds[i].fd << " not found." << std::endl;
						continue;
					}
					handle_client_message(user);
				}
			}
		}
	}
	CloseFds();
}
