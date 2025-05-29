/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:53:11 by p                 #+#    #+#             */
/*   Updated: 2025/05/29 22:23:58 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ChannelPrivate.hpp"
#include "ChannelPublic.hpp"


// Constructors
Server::Server() : _server_fd(-1), _fd_max(0)
{
	init_server_socket();
	std::cout << "Server created." << std::endl;
}

Server::Server(int server_fd, int fd_max)
{
	set_server_fd(server_fd);
	set_fd_max(fd_max);
	init_server_socket();
	std::cout << "Server created." << std::endl;
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
	for( std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it )
		//close( *it );
		delete *it;

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

void	Server::set_server_fd(int server_fd)
{
	this->_server_fd = server_fd;
}

int		Server::get_server_fd()
{
	return this->_server_fd;
}

void	Server::set_fd_max(int fd_max)
{
	this->_fd_max = fd_max;
}

int		Server::get_fd_max()
{
	return this->_fd_max;
}

void	Server::set_master_set(fd_set master_set)
{
	this->_master_set = master_set;
}

fd_set	Server::get_master_set()
{
	return this->_master_set;
}

void	Server::set_read_fds(fd_set read_fds)
{
	this->_read_fds = read_fds;
}

fd_set	Server::get_read_fds()
{
	return this->_read_fds;
}

Client	*Server::getClientByFd(int fd)
{
	// Search for the client by its file descriptor
	// Buscar el cliente por su descriptor de archivo
	for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->getFd() == fd)
		{
			return *it; // Return the found client
		}
	}
	std::cerr << "Client with fd " << fd << " not found." << std::endl;
	return NULL; // Exit if the client is not found
}

AChannel	*Server::getChannelByName(const std::string &channelName)
{
	// Search for the channel by its name
	// Buscar el canal por su nombre
	std::map<std::string, AChannel*>::iterator it = _channel_list.find(channelName);
	if (it != _channel_list.end())
		return it->second; // Return the found channel
	std::cerr << "Channel not found: " << channelName << std::endl;
	return NULL; // Exit if the channel is not found
}

/// @brief server socket initializer function
/// función para inicializar el socket del servidor
void	Server::init_server_socket()
{

	// TCP IP4 socket creation
	// Creación del socket TCP IP4
	set_server_fd( socket( AF_INET, SOCK_STREAM, 0 ));
	if ( get_server_fd() < 0 )
	{
		std::cerr << "Error al crear el socket." << std::endl;
		exit (1);
	}

	// server address scrtuct
	// Estructura para la dirección del servidor
	sockaddr_in server_addr;
	std::memset( &server_addr, 0, sizeof( server_addr) );	// struct cleaning
	server_addr.sin_family = AF_INET;						// IP4
	server_addr.sin_addr.s_addr = INADDR_ANY;				// accept conetions from all IP
	server_addr.sin_port = htons( PORT );					// port conversion to net format

	// Allow immediate port reuse after shutdown
	// Permitir reutilizar puerto innmediato despues de cierre
	int opt = 1;
	if ( setsockopt( get_server_fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt )) < 0 )
	{
		std::cerr << "Error en setsockopt()." << std::endl;
		close ( get_server_fd() );
		exit (1);
	}

	// Associate the socket yo an IP / port
	// Asociar el socket a una IP / puerto
	if ( bind( get_server_fd(), ( struct sockaddr* ) &server_addr, sizeof( server_addr )) < 0 )
	{
		std::cerr << "Error en bind()." << std::endl;
		close( get_server_fd() );
		exit(1);
	}

	// listen for incoming connections
	// escuchar conexiones entrantes
	if( listen(get_server_fd(), BACKLOG) < 0 )
	{
		std::cerr << "Error en listen()." << std::endl;
		close ( get_server_fd() );
		exit (1);
	}

	std::cout << "Servidor iniciado en el puerto " << PORT << std::endl;

	// Initialize the descriptor sets
	// Inicializar los conjuntos de descriptores
	FD_ZERO( &_master_set );					// clean master set
	FD_SET( get_server_fd(), &_master_set );	// add the socket server
	set_fd_max(get_server_fd());				// The current maximum is that of the server

}

/// @brief handle a new incoming conecction
/// manejar una nueva conexión entrante
void	Server::handle_new_connection()
{
	sockaddr_in	client_addr;
	socklen_t	addrlen = sizeof( client_addr );

	// Accept the new conexion and get its socket
	// Aceptar la nueva conexión y obtener su socket
	int			new_fd = accept( get_server_fd(), ( struct sockaddr* )&client_addr, &addrlen );

	if (new_fd < 0)
	{
		std::cerr << "Error en accept()." << std::endl;
	}
	else
	{
		// add the new client to the set
		// añadir el nuevo cliente al conjunto
		FD_SET( new_fd, &_master_set );
		if (new_fd > get_fd_max())
			set_fd_max(new_fd);					// refresch the higer fd

		Client* new_client = new Client(new_fd);				// create a new client object
		_clients.push_back(new_client);		// add the new client to the vector

		std::cout << "Nueva conexión desde " << inet_ntoa( client_addr.sin_addr ) << " en socket " << new_fd << std::endl;

	}
}

/// @brief Read and process a message from a client
///			leer y procesar un mensaje de un cleinte
/// @param client_fd
void	Server::handle_client_message(Client *client)
{
	char	buffer[BUFFER_SIZE];
	int		nbytes = recv( client->getFd(), buffer, BUFFER_SIZE - 1, 0 ); // data receiv

	if (nbytes < 0)
	{
		perror("recv");		// mostrar error exacto
		close( client->getFd() );
		FD_CLR( client->getFd(), &_master_set );
	}
	else if(nbytes == 0)
	{
		// if an error ocurr or the client desconetion
		std::cout << "Socket " << client->getUsername() << " disconected for the client." << std::endl;

		// close the client socket and remove it from the set
		close( client->getFd() );
		FD_CLR( client->getFd(), &_master_set );
	}
	else
	{
		buffer[nbytes] = '\0';		// Ensure the message end at \0
		std::cout << "Mensaje recibido de " << client->getUsername() << ": " << buffer;

		// handle the buffer
		// tratar el buffer
		//msg_handler::handle_buffer<int>(buffer);
		//msg_andler::andle_buffer<std::string>(buffer);
		std::string message(buffer);
		if(message.find("JOIN" ) == 0)
		{
			std::string channelName = message.substr(5);
			channelName.erase(channelName.find_last_not_of(" \n\r\t") + 1);
			joinChannel(channelName, client);
		}

		if(message.find("show") == 0)
		{
			std::cout << "Canales" << std::endl;
			for (std::map<std::string, AChannel*>::iterator it = _channel_list.begin(); it != _channel_list.end(); ++it)
			{
				int i = it->second->getMembers().size();
				std::cout << it->first << " - " << i << std::endl;
			}
		}

		// send a eco response to the client
		std::string response = "Mensaje recibido.\n";
		send( client->getFd(), response.c_str(), response.size(), 0 );
	}

}

/// @brief join or create a channel
///			entrar o crear un canal
/// @param channelName
/// @param new_client
void				Server::joinChannel(const std::string channelName, Client *new_client)
{
	AChannel* channel = getChannelByName(channelName);

	std::string msg;
	if (!channel) {
		// Crear nuevo canal público
		AChannel* newChannel = new ChannelPublic(channelName);
		newChannel->addMember(new_client, "");  // primer miembro
		_channel_list[channelName] = newChannel;
		msg = "Joined channel " + channelName + "\n";
		std::cout << "Created channel: " << channelName << std::endl;
	}
	else
	{

		if (channel->isMember(new_client->getNickname()))
		{
			channel->addMember(new_client, ""); // Agregar al canal existente
			std::cout << "Client " << new_client->getFd() << " joined " << channelName << std::endl;
			msg = "Joined channel " + channelName + "\n";
		}
		else
		{
			std::cerr << "Error: Client already in channel " << channelName << std::endl;
			msg = "Error: Client already in channel " + channelName + "\n";
		}
	}

	// Mensaje de confirmación
	send(new_client->getFd(), msg.c_str(), msg.length(), 0);
}

/// @brief principal loop
///			bucle principal
void	Server::run()
{
	while (true)
	{
		set_read_fds(get_master_set());		// copy set from selec()

		// wait for activity on any of the sockets
		// esperar actividad en alguno de los sockets
		if(select(get_fd_max() + 1, &_read_fds, NULL, NULL, NULL ) == -1)
		{
			std::cerr << "Error en select()." << std::endl;
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
					Client *client = getClientByFd(i);
					
					if (client == NULL)
					{
						std::cerr << "Client with fd " << i << " not found." << std::endl;
						continue; // Skip if client not found
					}
					handle_client_message(client);	// message of a client
				}
			}
		}
	}
}
