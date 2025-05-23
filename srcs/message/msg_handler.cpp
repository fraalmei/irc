/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:39:07 by p                 #+#    #+#             */
/*   Updated: 2025/05/23 11:45:24 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/message/msg_handler.hpp"

msg_andler::msg_andler() {}

msg_andler::~msg_andler() {}

template<>
std::string		msg_andler::andle_buffer(char* buffer)
{
	(void) buffer;
	return "";
}

template<>
int				msg_andler::andle_buffer(char* buffer)
{
	(void) buffer;
	return 0;
}
