/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraalmei <fraalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:39:01 by p                 #+#    #+#             */
/*   Updated: 2025/11/02 17:19:06 by fraalmei         ###   ########.fr       */
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

		static int		handle_buffer(std::string buffer, User *user, Server *Server);
		static int		handle_password(std::string buffer, User *user, Server *Server);
		static int		handle_nickname(std::string buffer, User *user);
		static int		handle_username(std::string buffer, User *user);
	
	private:

		msg_handler(void);
		~msg_handler();

};

#endif