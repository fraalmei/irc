/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:53:45 by p                 #+#    #+#             */
/*   Updated: 2025/05/22 17:55:23 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

# include "../channels/channels.hpp"
# include "../message/msg_handler.hpp"

# include <iostream>
# include <cstring>			// memset()
# include <cstdlib>			// exit()
# include <unistd.h>		// close()
# include <sys/types.h>		// Basic types for sockets
# include <sys/socket.h>	// Basic functions for sockets
# include <netinet/in.h>	// Structs for IP direccions IP4
# include <arpa/inet.h>		// inet_ntoa(), IP converion <> text
# include <sys/select.h>	// Multiplex
# include <cstdio>
# include <vector>
# include <map>
# include <algorithm>

# define PORT 6667		// listening port (IRC standard port)
# define BACKLOG 10		// Max number of waiting conetions
# define BUFFER_SIZE 512	// size of the buffer to receive messages

class Server
{
	public:

		// Constructors
		Server(void);
		Server(int server_fd, int fd_max);
		Server(const Server &copy);

		// Destructor
		~Server(void);
		
		// Operators
		Server & operator=(const Server &assign);
	
		void				set_server_fd(int server_fd);
		int					get_server_fd();
		void				set_fd_max(int fd_max);
		int					get_fd_max();
		void				set_master_set(fd_set master_set);
		fd_set				get_master_set();
		void				set_read_fds(fd_set read_fds);
		fd_set				get_read_fds();

		void				run(void);

	private:
		int									_server_fd;		//ecrver file descriptor
		fd_set								_master_set;	// set of file descriptors
		fd_set								_read_fds;		// temporal set to read
		int									_fd_max;		// high fd used

		void				init_server_socket();
		void				handle_new_connection();
		void				handle_client_message(int client_fd);
		void				joinChannel(const std::string channelName, int client_fd);

};

	// overload an operator
	std::ostream	&operator<<(std::ostream &o, Server const &i);

#endif
