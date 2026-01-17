/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:53:11 by p                 #+#    #+#             */
/*   Updated: 2026/01/17 20:30:12 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../include/prints.hpp"

/// @brief handle a new incoming conecction
/// manejar una nueva conexión entrante
void	Server::handle_new_connection()
{
	struct sockaddr_in	client_addr;
	socklen_t	addrlen = sizeof( client_addr );

	// Accept the new conexion and get its socket
	// Aceptar la nueva conexión y obtener su socket
	int			new_fd = accept( get_server_fd(), ( sockaddr* )&client_addr, &addrlen );
	if (new_fd == -1)
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en accept()." << std::endl;
		return;
	}
	if (fcntl( new_fd, F_SETFL, O_NONBLOCK ) == -1 )	// set the socket option (O_NONBLOCK) for non-blocking socket
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en fcntl()." << std::endl;
		close ( new_fd );
		return;
	}

	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nuevo cliente con fd " << new_fd << " aceptado y direccion " << client_addr.sin_addr.s_addr << "." << std::endl;

	// add the new client socket to the poll list so we can monitor it
	struct pollfd clientPoll;
	clientPoll.fd = new_fd;
	clientPoll.events = POLLIN;
	clientPoll.revents = 0;

	User* new_client = new User(new_fd);                // create a new user object
	new_client->setIpAdd(inet_ntoa( client_addr.sin_addr )); // set the client IP address

	// register new client in the clients map
	getClientList()[new_fd] = new_client;

	// register new client in the pollfd list
	_fds.push_back(clientPoll);

	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nueva conexión desde " << inet_ntoa( client_addr.sin_addr ) << " en socket " << new_fd << std::endl;

	//std::string welcome = "Contraseña. Por favor.\r\n";
	//send(new_fd, welcome.c_str(), welcome.size(), 0);
}

/// @brief Recibe un mensaje y lo pasa por el buffer handler, que lo va guardando en el objeto User hasta que esté completo
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
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " No data available (EAGAIN), continuing." << std::endl;
		}
		else
		{
			// Real error or disconnection
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error in recv: " << strerror(errno) << std::endl;
			ClearClients(user->getFd());
			print_users();
		}
	}
	else if (nbytes == 0)
	{
		// Client disconnected
		// Desconexión del cliente
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Client disconnected (nbytes == 0)." << std::endl;
		ClearClients(user->getFd());
		print_users();
	}

	prints::printchars(initbuffer);	// print ascii values of received message
	std::string buffer(initbuffer);	// convert to std::string
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Received raw buffer: '" << buffer << "' with " << nbytes << " bytes." << std::endl;

	user->addToBuffer(buffer);

	while (!user->getBuffer().empty() && user->getBuffer().find("\r\n") != std::string::npos) {
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Processing buffer: '" << user->getBuffer() << "'" << std::endl;
		msg_handler::t_command parsed_command = msg_handler::parse_msg(user);
		if (parsed_command.user != NULL) {
			msg_handler::execute_command(parsed_command, *this);
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

///		principal loop
///		bucle principal
void	Server::run()
{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Running server with fd -> " << get_server_fd() << " ." << std::endl;
	while (!get_signal())
	{
		// wait for activity on any of the sockets
		// esperar actividad en cualquiera de los sockets
		if ((poll(&_fds[0], _fds.size(), -1) == -1) && !get_signal())
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " poll() error." << std::endl;
			exit(1);
		}

		// check all fds in the poll list
		// comprobar todos los fds en la lista de poll
		for(size_t i = 0; i < _fds.size(); i++)
		{
			// check if there is activity on socket i
			// comprobar si hay actividad en el socket i
			if (_fds[i].revents & POLLIN)	// 
			{
				// if the activity is on the server socket, it is a new connection
				// si la actividad es en el socket del servidor, es una nueva conexión
				if (_fds[i].fd == get_server_fd())		
				{
					// new incoming conection
					// nueva conexión entrante
					handle_new_connection();
				}
				else
				{
					// message from a client
					// mensaje de un cliente
					User *user = getClientByFd(_fds[i].fd);
					if (user == NULL)
					{
						// This should not happen, but just in case
						// Esto no debería pasar, pero por si acaso
						std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Client with fd " << _fds[i].fd << " not found." << std::endl;
						continue;
					}
					handle_client_message(user); // message of a user

				}
			}
		}
	}
	CloseFds();
}
