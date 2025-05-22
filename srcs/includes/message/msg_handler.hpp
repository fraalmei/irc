/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:39:01 by p                 #+#    #+#             */
/*   Updated: 2025/05/22 11:51:43 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HANDLER_HPP
# define MSG_HANDLER_HPP

#include <string>

class	msg_andler
{
	public:

		template<typename T>
		static T		andle_buffer(char* buffer);
	
	private:
	
		msg_andler(void);
		~msg_andler();

};

#endif