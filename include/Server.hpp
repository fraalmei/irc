/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraalmei <fraalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:53:45 by p                 #+#    #+#             */
/*   Updated: 2025/11/02 15:01:51 by fraalmei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

# include "OChannel.hpp"
# include "msg_handler.hpp"

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

# define BACKLOG 10		// Max number of waiting conetions
# define BUFFER_SIZE 512	// size of the buffer to receive messages

class Server
{
	public:

		// Constructors
		Server(void);
		Server(char *password, int port);
		Server(const Server &copy);

		// Destructor
		~Server(void);
		
		// Operators
		Server & operator=(const Server &assign);
	
		// Getters
		int					get_server_fd() { return this->_server_fd; }
		int					get_fd_max() { return this->_fd_max; }
		fd_set				get_master_set() { return this->_master_set; }
		fd_set				get_read_fds() { return this->_read_fds; }
		int					get_port() { return this->_port; }
		std::string			get_password() { return this->_password; }

		const std::map<std::string, OChannel*>&		getConstChannelList() const { return this->_channel_list; }
		std::map<std::string, OChannel*>&			getChannelList() { return this->_channel_list; }
		const std::map<int, User*>&					getConstClientList() const { return this->_client_list; }
		std::map<int, User*>&						getClientList() { return this->_client_list; }

		User				*getClientByFd(int fd);
		OChannel			*getChannelByName(const std::string &channelName);

		// Setters
		void				set_server_fd(int server_fd) { this->_server_fd = server_fd; }
		void				set_fd_max(int fd_max) { this->_fd_max = fd_max; }
		void				set_master_set(fd_set master_set) { this->_master_set = master_set; }
		void				set_read_fds(fd_set read_fds) { this->_read_fds = read_fds; }

		void				run(void);

		void				joinChannel(const std::string channelName, User *new_client);

	private:

		int									_server_fd;			//	server file descriptor
		fd_set								_master_set;		//	set of file descriptors
		fd_set								_read_fds;			//	temporal set to read
		int									_fd_max;			//	high fd used

		int									_port;				//	port number	
		std::string							_password;			//	server password

		std::map<int, User*>					_client_list;		// fd client list
		//std::vector<Client*>					_clients;			// vector of clients
		std::map<std::string, OChannel*>		_channel_list;		// map of the created channels

		void				init_server_socket();
		void				handle_new_connection();
		void				handle_client_message(User *client);

};

	// overload an operator
	std::ostream	&operator<<(std::ostream &o, Server const &i);

#endif
