/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:53:11 by p                 #+#    #+#             */
/*   Updated: 2025/11/16 14:23:09 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "colors.hpp"

/// @brief server socket initializer function
/// función para inicializar el socket del servidor
void	Server::init_server_socket()
{

	// TCP IP4 socket creation
	// Creación del socket TCP IP4
	set_server_fd( socket( AF_INET, SOCK_STREAM, 0 ));
	if ( get_server_fd() < 0 )
	{
		std::cerr << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error al crear el socket." << std::endl;
		exit (1);
	}

	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Socket creado con fd " << get_server_fd() << " ." << std::endl;

	// server address scrtuct
	// Estructura para la dirección del servidor
	sockaddr_in server_addr;
	std::memset( &server_addr, 0, sizeof( server_addr) );	// struct cleaning
	server_addr.sin_family = AF_INET;						// IP4
	server_addr.sin_addr.s_addr = INADDR_ANY;				// accept conetions from all IP
	server_addr.sin_port = htons( _port );					// port conversion to net format

	// Allow immediate port reuse after shutdown
	// Permitir reutilizar puerto innmediato despues de cierre
	int opt = 1;
	if ( setsockopt( get_server_fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt )) < 0 )
	{
		std::cerr << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en setsockopt()." << std::endl;
		close ( get_server_fd() );
		exit (1);
	}

	// Associate the socket to an IP / port
	// Asociar el socket a una IP / puerto
	if ( bind( get_server_fd(), ( struct sockaddr* ) &server_addr, sizeof( server_addr )) < 0 )
	{
		std::cerr << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en bind()." << std::endl;
		close( get_server_fd() );
		exit(1);
	}

	// listen for incoming connections
	// escuchar conexiones entrantes
	if( listen(get_server_fd(), BACKLOG) < 0 )
	{
		std::cerr << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en listen()." << std::endl;
		close ( get_server_fd() );
		exit (1);
	}

	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Servidor iniciado en el puerto " << _port << std::endl;
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Servidor iniciado con contraseña: '" << _password << "'" << std::endl;

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
		std::cerr << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en accept()." << std::endl;
	}
	else
	{

		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nuevo cliente con fd " << new_fd << " aceptado y direccion " << client_addr.sin_addr.s_addr << "." << std::endl;

		// add the new user to the set
		// añadir el nuevo cliente al conjunto
		FD_SET( new_fd, &_master_set );
		if (new_fd > get_fd_max())
			set_fd_max(new_fd);					// refresch the higer fd

		User* new_client = new User(new_fd);				// create a new user object
		
		//_clients.push_back(new_client);		// add the new user to the vector
		getClientList()[new_fd] = new_client;		// add the new user to the vector

		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Nueva conexión desde " << inet_ntoa( client_addr.sin_addr ) << " en socket " << new_fd << std::endl;

		std::string welcome = "CAP * LS : \r\n";
		send(new_fd, welcome.c_str(), welcome.size(), 0);
	}
}

/// @brief Recibe un mensaje y lo pasa por el buffer handler, que lo va guardando en el objeto User hasta que esté completo
/// @param client_fd
int	Server::handle_client_message(User *user)
{
	//std::string	buffer;
	//int			nbytes = recv( user->getFd(), &buffer[0], BUFFER_SIZE - 1, 0 ); // data receiv
	char	initbuffer[BUFFER_SIZE];
	int		nbytes = recv( user->getFd(), initbuffer, BUFFER_SIZE - 1, 0 ); // data receiv

	// Remove trailing whitespace characters (\t, \r)
	while (nbytes > 0 && (initbuffer[nbytes - 1] == '\t' || 
                      initbuffer[nbytes - 1] == '\r') ) {
    	nbytes--;
	}
	initbuffer[nbytes] = '\0';
	std::string buffer(initbuffer);

	//buffer.resize(BUFFER_SIZE);
	if (user->getNickname() != "")
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Se ha recibido mensaje de " << user->getNickname() << ": '" << buffer <<  "' con " << buffer.size() << " chars and " << nbytes << " bytes." << std::endl;
	if (nbytes < 0)
    {
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " nbytes < 0" << std::endl;
        perror("recv");		// mostrar error exacto
        close( user->getFd() );
        FD_CLR( user->getFd(), &_master_set );
		return 1;
    }
    else if(nbytes == 0)
    {
        // if an error ocurr or the client desconetion
        std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Socket " << user->getNickname() << " disconected for the user." << std::endl;

        // close the user socket and remove it from the set
        close( user->getFd() );
        FD_CLR( user->getFd(), &_master_set );
		return 1;
    }
    else
    {
        //buffer.resize(nbytes);                      // ajustar al tamaño real recibido
        //buffer.push_back('\0');                      // asegurar terminador si lo necesita el código siguiente
		if (user->getNickname() != "")
        	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Se ha recibido mensaje de " << user->getNickname() << ": '" << buffer <<  "' con " << buffer.size() << " chars and " << nbytes << " bytes." << std::endl;
		
		if (!msg_handler::handle_buffer(buffer, user)) // handle the buffer
		{
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje parcial." << std::endl;
			return 1;
		}
		// send a eco response to the user
		else
		{
			std::string	response;
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje recibido." << std::endl;
			response = "Mensaje recibido.\n";
			send(user->getFd(), response.c_str(), response.size(), 0);
			return 0;
		}
    }
	return 0;
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
				std::cerr << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error: User already in channel " << channelName << std::endl;
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " User " << new_client->getFd() << " joined " << channelName << std::endl;
			msg = "Joined channel " + channel->getName() + "\n";
		}
		else
		{
			std::cerr << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error: User already in channel " << channelName << std::endl;
			msg = "Error: User already in channel " + channelName + "\n";
		}
	}

	// Mensaje de confirmación
	send(new_client->getFd(), msg.c_str(), msg.length(), 0);
}

///			principal loop
///			bucle principal
void	Server::run()
{

	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Running server." << std::endl;
	while (true)
	{
		set_read_fds(get_master_set());		// copy set from selec()

		// select() is used to wait for activity on any of the sockets
		// int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
		// nfds: highest file descriptor + 1
		// readfds: set of file descriptors to be monitored for reading
		// writefds: set of file descriptors to be monitored for writing
		// exceptfds: set of file descriptors to be monitored for exceptional conditions
		// timeout: maximum time to wait for activity (NULL means wait indefinitely)
		
		// select() se usa para esperar actividad en alguno de los sockets
		// int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
		// nfds: el mayor descriptor de archivo + 1
		// readfds: conjunto de descriptores de archivo a monitorear para lectura
		// writefds: conjunto de descriptores de archivo a monitorear para escritura
		// exceptfds: conjunto de descriptores de archivo a monitorear para condiciones excepcionales
		// timeout: tiempo máximo de espera para actividad (NULL significa esperar indefinidamente)

		if(select(get_fd_max() + 1, &_read_fds, NULL, NULL, NULL ) == -1)
		{
			std::cerr << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Error en select()." << std::endl;
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
						std::cerr << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Client with fd " << i << " not found." << std::endl;
					else if (this->handle_client_message(user))
						break;	// incomplete message, wait for more data
					if (!user->isAuthenticated())
						msg_handler::handle_login_parse(user, this); //should only read login messages until complete
					else
						msg_handler::parse_msg(user->getBuffer()); //It is a msg from an active user, parse the command
				}
			}
		}
	}
}
