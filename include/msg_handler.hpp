/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 17:33:06 by p                 #+#    #+#             */
/*   Updated: 2025/12/14 18:28:14 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HANDLER_HPP
# define MSG_HANDLER_HPP

# include <string>
# include <iostream>
# include <ostream>
# include <list>

class Server;
class User;

# include "colors.hpp"

class	msg_handler
{
	public:
		// Command structure used for parsing and execution
		typedef struct s_command
		{
			User*                   user;
			std::string             command;
			std::list<std::string>  params;
		}   t_command;

		static int			handle_buffer(std::string buffer, User *user);
		static t_command	parse_msg(std::string msg);
		static void 	execute_command(t_command command);
		static int			aunthenticateUser(User *user, Server *server);
		static int				print_command(t_command command);

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