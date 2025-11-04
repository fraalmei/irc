/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:39:01 by p                 #+#    #+#             */
/*   Updated: 2025/11/04 15:57:22 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HANDLER_HPP
# define MSG_HANDLER_HPP

# include <string>
# include <iostream>
# include <ostream>

class Server;
class User;

# include "Server.hpp"
# include "User.hpp"

class	msg_handler
{
	public:

		static int		handle_buffer(std::string buffer, User *user);
		static int		handle_password(std::string buffer, User *user, Server *Server);
		static int		handle_nickname(std::string buffer, User *user);
		static int		handle_username(std::string buffer, User *user);
		static int		parse_msg(User *user);
	
	private:
		msg_handler(void);
		~msg_handler();

};

#endif