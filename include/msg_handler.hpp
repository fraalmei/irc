/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:39:01 by p                 #+#    #+#             */
/*   Updated: 2025/07/24 13:37:20 by p                ###   ########.fr       */
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

		template<typename T>
		static T		handle_buffer(char* buffer, User *user, Server *Server);
	
	private:

		msg_handler(void);
		~msg_handler();

};

#endif