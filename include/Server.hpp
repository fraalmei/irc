/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:53:45 by p                 #+#    #+#             */
/*   Updated: 2025/07/11 12:23:06 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

# include "AChannel.hpp"
# include "msg_handler.hpp"
# include "ChannelPrivate.hpp"
# include "ChannelPublic.hpp"

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
	
		// Getters
		int					get_server_fd();
		int					get_fd_max();
		fd_set				get_master_set();
		fd_set				get_read_fds();

		const std::map<std::string, AChannel*>&		getConstChannelList() const;
		std::map<std::string, AChannel*>&			getChannelList();
		const std::map<int, User*>&		getConstClientList() const;
		std::map<int, User*>&				getClientList();

		User				*getClientByFd(int fd);
		AChannel			*getChannelByName(const std::string &channelName);

		// Setters
		void				set_server_fd(int server_fd);
		void				set_fd_max(int fd_max);
		void				set_master_set(fd_set master_set);
		void				set_read_fds(fd_set read_fds);

		void				run(void);

		void				joinChannel(const std::string channelName, User *new_client);

	private:

		int									_server_fd;			//	server file descriptor
		fd_set								_master_set;		//	set of file descriptors
		fd_set								_read_fds;			//	temporal set to read
		int									_fd_max;			//	high fd used

		std::map<int, User*>					_client_list;			// fd client list
		//std::vector<Client*>					_clients;			// vector of clients
		std::map<std::string, AChannel*>		_channel_list;		// map of the created channels

		void				init_server_socket();
		void				handle_new_connection();
		void				handle_client_message(User *client);

};

	// overload an operator
	std::ostream	&operator<<(std::ostream &o, Server const &i);

#endif
