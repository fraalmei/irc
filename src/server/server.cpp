/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:53:11 by p                 #+#    #+#             */
/*   Updated: 2025/12/10 11:03:29 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
		{std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en accept()." << std::endl; return;}

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

	std::string welcome = "Contraseña. Por favor.\n";
	send(new_fd, welcome.c_str(), welcome.size(), 0);
}

/// @brief Recibe un mensaje y lo pasa por el buffer handler, que lo va guardando en el objeto User hasta que esté completo
/// @param client_fd
std::string	Server::handle_client_message(User *user)
{
	char	initbuffer[BUFFER_SIZE];
	memset(initbuffer, 0, sizeof(initbuffer)); // clear the buffer
	ssize_t	nbytes = recv( user->getFd(), initbuffer, BUFFER_SIZE - 1, 0 ); // data receiv

	// Remove trailing whitespace characters (\t, \r)
	if (nbytes <= 0 )
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " nbytes <= 0" << std::endl;
		perror("recv");		// mostrar error exacto
		ClearClients(user->getFd());
		close( user->getFd() );
		return "";
	}
	prints::printchars(initbuffer);
	initbuffer[nbytes] = '\0';
	std::string buffer(initbuffer);
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Received raw buffer: '" << buffer << "' with " << nbytes << " bytes." << std::endl;
	
	//rtrim_crlf(buffer);
	//std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Received trimed buffer: '" << buffer << "' with " << nbytes << " bytes." << std::endl;

	// send a eco response to the user
	std::string	response;
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje recibido." << std::endl;
	response = "Mensaje recibido.\n";
	send(user->getFd(), response.c_str(), response.size(), 0);
	// add the received buffer to the user buffer
	
	//user->clearBuffer();
	if (msg_handler::handle_buffer(buffer, user))
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje vacio, erroneo o incompleto." << std::endl;
		return ""; // mensaje incompleto, esperar más datos"";
	}
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Stablized user buffer: '" << user->getBuffer() << "' with " << nbytes << " bytes." << std::endl;
	return user->getBuffer();
}

/// @brief join or create a channel
///			entrar o crear un canal
/// @param channelName
/// @param new_client
void				Server::joinChannel(const std::string channelName, User *new_client)
{
	Channel* channel = getChannelByName(channelName);

	std::string msg;
	if (channel == NULL)
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " "<< channelName << " not found" << std::endl;
		// Crear nuevo canal público
		Channel* newChannel = new Channel(channelName);
		newChannel->addMember(new_client);	// primer miembro
		getChannelList()[channelName] = newChannel;
		msg = "Joined channel " + newChannel->getName() + "\n";
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Created channel: " << channelName << std::endl;
	}
	else
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Found channel: " << channel->getName() << std::endl;
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " " << channel->isMember(new_client->getFd()) << std::endl;
		if (!channel->isMember(new_client->getFd()))
		{
			if (channel->addMember(new_client) != 0 ) // Agregar al canal existente
				std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error: User already in channel " << channelName << std::endl;
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " User " << new_client->getFd() << " joined " << channelName << std::endl;
			msg = "Joined channel " + channel->getName() + "\n";
		}
		else
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error: User already in channel " << channelName << std::endl;
			msg = "Error: User already in channel " + channelName + "\n";
		}
	}

	// Mensaje de confirmación
	send(new_client->getFd(), msg.c_str(), msg.length(), 0);
}

/// @brief Signal handler for graceful shutdown
void	Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	set_signal(true);	// set the static boolean to true to stop the server
}


///		principal loop
///			bucle principal
void	Server::run()
{

	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Running server with fd -> " << _server_fd << " ." << std::endl;
	while (!get_signal())
	{

		if ((poll(&_fds[0], _fds.size(), -1) == -1) && !get_signal())
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en poll()." << std::endl;
			exit(1);
		}

		for(size_t i = 0; i < _fds.size(); i++)	// check all fds in the poll list
		{
			if (_fds[i].revents & POLLIN)	// 
			{
				if (_fds[i].fd == get_server_fd())		// if there is activity on socket i
				{
					handle_new_connection();	// new incoming conection
				}
				else if(getClientByFd(_fds[i].fd)->isAuthenticated())
				{
					if(handle_client_message(getClientByFd(_fds[i].fd)).empty())
					{
						continue;	// mensaje incompleto, esperar más datos
					}
					msg_handler::parse_msg(getClientByFd(_fds[i].fd)->getBuffer());
					getClientByFd(_fds[i].fd)->clearBuffer();
				}
				else
					msg_handler::aunthenticateUser(getClientByFd(_fds[i].fd), this);
				
				/*	if (user == NULL)
						std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Client with fd " << _fds[i].fd << " not found." << std::endl;
					else
					{
						if (this->handle_client_message(user))
							break;	// message of a user
					}
					if (!user->isAuthenticated())
					{
						msg_handler::handle_login_parse(user, this);
					}
					else
						msg_handler::parse_msg(user->getBuffer()); //It is a msg from an active user, parse the command
				
				}*/
			}
		}
/*
		if(select(get_fd_max() + 1, &_read_fds, NULL, NULL, NULL ) == -1)
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en select()." << std::endl;
			exit(1);
		}

		// traverse all posible fd
		// recorrer todos los fd posibles
		for(int i = 0; i <= get_fd_max(); ++i)
		{
			if(FD_ISSET(i, &_read_fds))
			{
				if(i == get_server_fd())		// if there is activity on socket i
				{
					handle_new_connection();	// new incoming conection
				}
				else
				{
					User *user = getClientByFd(i);

					if (user == NULL)
						std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Client with fd " << i << " not found." << std::endl;
					else
					{
						if (this->handle_client_message(user))
							break;	// message of a user
					}
					if (!user->isAuthenticated())
					{
						msg_handler::handle_login_parse(user, this);
						// METERLO EN LA FUNCION DE ARRIBA
						// std::cout << "Buffer recibido de " << user->getFd() << ": " << buffer;
						// std::string	message = buffer.substr(0, buffer.size() - 3); // remove \n\r
						// std::cout << "Mensaje recibido de " << user->getFd() << ": " << message;
						// if (user->parse_user(message, this)) // handle the password
						// 	std::cout << "Mensaje tratado incorrectamente." << std::endl;
						
					}
					else
						msg_handler::parse_msg(user->getBuffer()); //It is a msg from an active user, parse the command
				}

			}
		}
*/
	}
	CloseFds();
}
