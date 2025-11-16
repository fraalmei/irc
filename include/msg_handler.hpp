/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 17:33:06 by p                 #+#    #+#             */
/*   Updated: 2025/11/16 11:49:36 by p                ###   ########.fr       */
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
# include "colors.hpp"

class	msg_handler
{
	public:

		static int		handle_buffer(std::string buffer, User *user);
		static int		parse_msg(std::string msg);
		
		static int		handle_login_parse(User *user, Server *server);

	private:
		msg_handler(void);
		~msg_handler();

		// Parse user
		static int		parse_user(std::string buffer, User *user, Server *server);
		static int		handle_password(std::string buffer, User *user, Server *server);
		static int		handle_nickname(std::string buffer, User *user);
		static int		handle_username(std::string buffer, User *user);

};

#endif