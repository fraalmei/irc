/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 17:33:06 by p                 #+#    #+#             */
/*   Updated: 2026/01/17 17:01:24 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HANDLER_HPP
# define MSG_HANDLER_HPP

# include <string>
# include <iostream>
# include <ostream>
# include <sstream>
# include <list>
# include <iterator>

class Server;
class User;

# include "colors.hpp"
# include "Server.hpp"

class	msg_handler
{
	public:
		// Command structure used for parsing and execution
		typedef struct s_command
		{
			User*                   user;
			std::string			 	actual_line;
			std::string				prefix;
			std::string             command;
			std::list<std::string>  params;
		}   t_command;

		static int			handle_buffer(std::string buffer, User *user);
		static t_command	parse_msg(User* user);
		static void 		execute_command(t_command command, Server &server);
		static int			aunthenticateUser(User *user, Server *server);
		static int			print_command(t_command command);

	private:
		msg_handler(void);
		~msg_handler();

		// Parse user authentication steps
		static int			authenticate(User *user, Server *server);
		static int			handle_password(msg_handler::t_command command, Server *server);
		static int			handle_nickname(msg_handler::t_command command);
		static int			handle_username(msg_handler::t_command command);

		// Other command handlers can be added here
		static int			joinChannel(const std::string channelName, User *new_client, Server &server);

};

// IRC Response Codes
#define RPL_WELCOME 001
#define RPL_YOURHOST 002
#define RPL_CREATED 003
#define RPL_MYINFO 004
#define RPL_TOPIC 332
#define RPL_NAMREPLY 353
#define RPL_ENDOFNAMES 366
#define ERR_NOSUCHCHANNEL 403
#define ERR_BADCHANNELKEY 475
#define ERR_NOTONCHANNEL 442
#define ERR_NICKNAMEINUSE 433
#define ERR_NONICKNAMEGIVEN 431
#define ERR_NEEDMOREPARAMS 461
#define ERR_PASSWDMISMATCH 464

#endif