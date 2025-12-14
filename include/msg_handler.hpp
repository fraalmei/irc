/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 17:33:06 by p                 #+#    #+#             */
/*   Updated: 2025/12/14 17:57:49 by samartin         ###   ########.fr       */
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

		static int			handle_buffer(std::string buffer, User *user);
		static t_command	parse_msg(std::string msg);
		static void 		execute_command(t_command command);
		static int			aunthenticateUser(User *user, Server *server);

	private:
		msg_handler(void);
		~msg_handler();

		// Parse user authentication steps
		static int			authenticate(User *user, Server *server);
		static int			handle_password(User *user, Server *server);
		static int			handle_nickname(User *user);
		static int			handle_username(User *user);

};

#endif