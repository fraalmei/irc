/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:53:45 by p                 #+#    #+#             */
/*   Updated: 2025/12/06 12:30:16 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Channel.hpp"
# include "msg_handler.hpp"
# include "User.hpp"
# include "colors.hpp"
# include <iostream>
# include <stdlib.h>		// exit()
# include <string.h> 		// memset()
# include <signal.h>		// signal()
# include <unistd.h>		// close()
# include <sys/types.h>		// Basic types for sockets
# include <sys/socket.h>	// Basic functions for sockets
# include <netinet/in.h>	// Structs for IP direccions IP4
# include <arpa/inet.h>		// inet_ntoa(), IP converion <> text
# include <sys/select.h>	// Multiplex
# include <fcntl.h>			// fcntl()
# include <poll.h>			// poll()
# include <cstdio>
# include <vector>
# include <map>
# include <algorithm>
# include <cerrno>

# define BACKLOG 10		// Max number of waiting conections
# define BUFFER_SIZE 512	// size of the buffer to receive messages

class User;
class Channel;

class Server
{
	public:
		// Orthodox functions
		Server(void);
		Server(char *password, int port);
		Server(const Server &copy);
		~Server(void);
		Server & operator=(const Server &assign);

		// Getters
		int					get_server_fd() { return this->_server_fd; }
		int					get_fd_max() { return this->_fd_max; }
		fd_set				get_master_set() { return this->_master_set; }
		fd_set*				get_master_set_ptr() { return &this->_master_set; }
		fd_set				get_read_fds() { return this->_read_fds; }
		int					get_port() { return this->_port; }
		std::string			get_password() { return this->_password; }
		bool				get_signal() { return _signal; }

		const std::map<std::string, Channel*>&	getConstChannelList() const { return this->_channel_list; }
		std::map<std::string, Channel*>&		getChannelList() { return this->_channel_list; }
		const std::map<int, User*>&				getConstClientList() const { return this->_clients; }
		std::map<int, User*>&					getClientList() { return this->_clients; }
		int										closeClientFd(int fd);
		User									*getClientByFd(int fd);
		User									*getClientFdByNickname(const std::string &nickname);	
		int										getClientCount() const { return this->_clients.size(); }
		Channel									*getChannelByName(const std::string &channelName);
		int										getChannelCount() const { return this->_channel_list.size(); }

		// Setters
		void				set_server_fd(int server_fd) { this->_server_fd = server_fd; }
		void				set_fd_max(int fd_max) { this->_fd_max = fd_max; }
		void				set_master_set(fd_set master_set) { this->_master_set = master_set; }
		static bool			set_signal(bool signal) { _signal = signal; return _signal; }	
		void				run(void);
		void				joinChannel(const std::string channelName, User *new_client);
		static void			SignalHandler(int signum);
		void				CloseFds();				// close file descriptors
		void				ClearClients(int fd);	// clear clients
		void				rtrim_crlf(std::string &s);
		std::string			handle_client_message(User *client);

	private:
		int									_server_fd;			//	server file descriptor
		fd_set								_master_set;		//	set of file descriptors
		fd_set								_read_fds;			//	temporal set to read
		int									_fd_max;			//	high fd used
		static bool							_signal;			//	signal to close the server	
		int									_port;				//	port number	
		std::string							_password;			//	server password

		// client list: map fd -> User*
		std::map<int, User*>				_clients;			// map of client pointers by fd
		std::map<std::string, Channel*>		_channel_list;		// map of the created channels
		std::vector<struct pollfd>			_fds;				//	vector of pollfd structures

		void				init_server_socket();
		void				handle_new_connection();
};

	// overload an operator
	std::ostream	&operator<<(std::ostream &o, Server const &i);
#endif
